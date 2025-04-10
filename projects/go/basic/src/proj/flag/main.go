package main

import (
        "flag"
        "fmt"
       )

func main() {
    var ipaddr = flag.String("ipaddr", "127.0.0.1", "IP 地址")
    flag.Parse()
    fmt.Println(*ipaddr)
}
