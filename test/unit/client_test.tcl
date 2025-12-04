puts "Startig the servers..."

set serverPid [exec ../../server-http &]
set host "127.0.0.1"
set port 6700

puts "tring test connection to $host:$port ..."

after 500

if { [catch {set sock  [socket $host $port]} err] } {
    puts "Errore: $err"
    exit 1
}

if {$tcl_platform(platform) == "unix"} {
    puts "Closing the server pids"
    if { [catch {exec kill $serverPid} msg] } {
    puts "Impossible to kill the server, errore in the code in happened"
    } else {
        puts "Server ended."
    }
}
