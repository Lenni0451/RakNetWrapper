package main

import (
	"C"
	"reflect"
	"sync"
	"sync/atomic"
	"time"
	"unsafe"

	"github.com/sandertv/go-raknet"
)

type CopyOfElasticChan[T any] struct {
	mu  sync.RWMutex
	len atomic.Int64
	ch  chan T
}

//export connect
func connect(address *C.char, timeout int64, connection *unsafe.Pointer) *C.char {
	clientConn, err := raknet.DialTimeout(C.GoString(address), time.Duration(timeout)*time.Millisecond)
	if err != nil {
		return C.CString(err.Error())
	} else {
		*connection = unsafe.Pointer(clientConn)
		return nil
	}
}

//export receivePacket
func receivePacket(connection unsafe.Pointer, data *unsafe.Pointer, length *int32) *C.char {
	packets := reflect.ValueOf((*raknet.Conn)(connection)).Elem().FieldByName("packets").Pointer()
	//Don't trust the IDE here, this pointer code is very safe and will not cause any problems ever
	packetsChanLen := reflect.ValueOf((*CopyOfElasticChan[[]byte])(unsafe.Pointer(packets))).Elem().FieldByName("ch").Len()
	if packetsChanLen == 0 {
		*data = nil
		*length = 0
		return nil
	}

	packet, err := (*raknet.Conn)(connection).ReadPacket()
	if err != nil {
		return C.CString(err.Error())
	} else {
		*data = C.CBytes(packet)
		*length = int32(len(packet))
		return nil
	}
}

//export sendPacket
func sendPacket(connection unsafe.Pointer, data unsafe.Pointer, length int32) *C.char {
	_, err := (*raknet.Conn)(connection).Write(unsafe.Slice((*byte)(data), length))
	if err != nil {
		return C.CString(err.Error())
	} else {
		return nil
	}
}

//export getLatency
func getLatency(connection unsafe.Pointer) int64 {
	return (*raknet.Conn)(connection).Latency().Milliseconds()
}

//export disconnect
func disconnect(connection unsafe.Pointer) *C.char {
	err := (*raknet.Conn)(connection).Close()
	if err != nil {
		return C.CString(err.Error())
	} else {
		return nil
	}
}

func main() {
}
