use std::error::Error;
use std::ffi::{c_char, c_void, CStr};
use std::io::ErrorKind;
use std::ptr::{null, null_mut};
use std::time::Duration;

use rust_raknet::{RaknetSocket, Reliability};
use rust_raknet::error::RaknetError;
use tokio::runtime::Runtime;
use tokio::time::timeout;

pub type DynResult<T> = Result<T, Box<dyn Error>>;

static mut RUNTIME: Option<Runtime> = None;

#[no_mangle]
pub extern "C" fn connect(address: *const c_char, connect_timeout: i32, raknet_version: u8, connection: *mut *const c_void) -> *const c_char {
    let result = sync_connect(address, connect_timeout, raknet_version, connection);
    if let Err(e) = result {
        to_c_string(&e.to_string())
    } else {
        null()
    }
}

fn sync_connect(address: *const c_char, connect_timeout: i32, raknet_version: u8, connection: *mut *const c_void) -> DynResult<()> {
    unsafe {
        let result = get_runtime()?.block_on(async_connect(to_rust_string(address)?, connect_timeout, raknet_version))?;
        let output = &mut *connection.cast::<&mut RaknetSocket>();

        *output = Box::leak(Box::new(result));
    }

    Ok(())
}

async fn async_connect(address: &str, connect_timeout: i32, raknet_version: u8) -> DynResult<RaknetSocket> {
    let resolved_address = tokio::net::lookup_host(address).await?.next();
    if resolved_address.is_none() {
        return Err("Could not resolve address".into());
    }
    let result = timeout(Duration::from_millis(connect_timeout as u64), RaknetSocket::connect_with_version(&resolved_address.unwrap(), raknet_version)).await;
    result?.map_err(map_err)
}

#[no_mangle]
pub extern "C" fn begin_receive_packet(connection: *const c_void, read_timeout: i32, read_callback: *const c_void) -> *const c_char {
    let result = sync_begin_receive_packet(connection as usize, read_timeout, read_callback as usize);
    if let Err(e) = result {
        to_c_string(&e.to_string())
    } else {
        null()
    }
}

fn sync_begin_receive_packet(connection: usize, read_timeout: i32, read_callback: usize) -> DynResult<()> {
    get_runtime()?.spawn(async move {
        unsafe {
            let callback = core::mem::transmute::<_, extern "C" fn(*const c_char, *const u8, i32)>(read_callback);
            loop {
                let result = core::mem::transmute::<_, &mut RaknetSocket>(connection).recv();
                let result = timeout(Duration::from_millis(read_timeout as u64), result).await;
                match result {
                    Ok(Ok(bytes)) => {
                        callback(null_mut(), bytes.as_ptr(), bytes.len() as i32);
                    }
                    Ok(Err(e)) => {
                        callback(to_c_string(&map_err(e).to_string()), null_mut(), 0);
                        break;
                    }
                    Err(_) => {
                        callback(to_c_string("Read timeout"), null_mut(), 0);
                        break;
                    }
                }
            }
        }
    });

    Ok(())
}

#[no_mangle]
pub extern "C" fn send_packet(connection: *const c_void, data: *const c_void, length: i32) -> *const c_char {
    let result = sync_send_packet(connection, data, length);
    if let Err(e) = result {
        to_c_string(&e.to_string())
    } else {
        null()
    }
}

fn sync_send_packet(connection: *const c_void, data: *const c_void, length: i32) -> DynResult<()> {
    unsafe {
        let bytes = core::slice::from_raw_parts(core::mem::transmute::<_, *const u8>(data), length as usize);
        get_runtime()?.block_on(async_send_packet(core::mem::transmute::<_, &mut RaknetSocket>(connection), bytes))?;
    }

    Ok(())
}

async fn async_send_packet(connection: &RaknetSocket, data: &[u8]) -> DynResult<()> {
    connection.send(data, Reliability::ReliableOrdered).await.map_err(map_err)?;
    Ok(())
}

#[no_mangle]
pub extern "C" fn disconnect(connection: *const c_void) -> *const c_char {
    let result = sync_disconnect(connection);
    if let Err(e) = result {
        to_c_string(&e.to_string())
    } else {
        null()
    }
}

fn sync_disconnect(connection: *const c_void) -> DynResult<()> {
    unsafe {
        get_runtime()?.block_on(async_disconnect(core::mem::transmute::<_, &mut RaknetSocket>(connection)))?;
    }

    Ok(())
}

async fn async_disconnect(connection: &RaknetSocket) -> DynResult<()> {
    connection.close().await.map_err(map_err)?;
    Ok(())
}

fn get_runtime() -> DynResult<&'static Runtime> {
    unsafe {
        if RUNTIME.is_none() {
            RUNTIME = Some(tokio::runtime::Builder::new_multi_thread().enable_all().build()?);
        }

        Ok(RUNTIME.as_ref().unwrap())
    }
}

fn map_err(e: RaknetError) -> Box<dyn Error> {
    std::io::Error::new(ErrorKind::Other, format!("{:?}", e)).into()
}

fn to_rust_string(string: *const c_char) -> DynResult<&'static str> {
    unsafe {
        Ok(CStr::from_ptr(string).to_str()?)
    }
}

fn to_c_string(string: &str) -> *const c_char {
    let mut error_string = string.to_string();
    error_string.push('\0');
    return String::leak(error_string).as_ptr() as _;
}
