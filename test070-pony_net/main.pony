use "buffered"
use "format"
use "net"

actor Main
  new create(env: Env) =>
    try
      let server = Server(TCPListenAuth(env.root as AmbientAuth), 12345, env.out)
    else
      env.err.print("Failed to start a server")
    end

actor Server
  let _listener: TCPListener

  new create(auth: TCPListenAuth, port: U16, log: OutStream) =>
    _listener = TCPListener(auth, recover ListenNotify(log) end
                            where service = port.string())
    log.print("Server started")

class ListenNotify is TCPListenNotify
  let _log: OutStream

  new create(log: OutStream) =>
    _log = log

  fun ref connected(listen: TCPListener ref): TCPConnectionNotify iso^ =>
    _log.print("A client connected")
    recover ConnectionNotify(_log) end

  fun ref not_listening(listen: TCPListener ref) =>
    None

class ConnectionNotify is TCPConnectionNotify
  let _log: OutStream

  new create(log: OutStream) =>
    _log = log

  fun ref received(conn: TCPConnection ref, data: Array[U8] iso, times: USize): Bool =>
    _log.print("Received " + data.size().string() + " bytes / " + times.string())
    true

  fun ref closed(conn: TCPConnection ref) =>
    _log.print("Connection closed")

  fun ref connect_failed(conn: TCPConnection ref) =>
    None
