# -*- coding: UTF-8 -*-
import frida
import sys
import time


jsInlineHook = """
Java.perform(function main() {
    var libart = Module.enumerateSymbols("libc.so");
    var addr = NULL;
    for (var n in libart) {
        //console.log("The func name: " + libart[n].name);
        
        if (libart[n].name == "malloc") {
            addr = libart[n].address;
            console.log("The malloc of func: " + addr);
            break;
        }
    }
    
    Interceptor.attach(addr, 
    {
        onEnter: function (arg) 
        {
            console.log("Enter the func!");
        },
        onLeave: function (retval) {
            console.log("Leave the func!");
        }
    })
});
"""

device = frida.get_usb_device()
pid = device.spawn(["com.check.hook"])
device.resume(pid)
session = device.attach(pid)
time.sleep(3)
script= session.create_script(jsInlineHook)
script.load()
sys.stdin.read()
