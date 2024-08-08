# -*- coding: UTF-8 -*-
import frida
import sys
import time

# 绕过TracerPid检测
jsHookTracerPid = """
Java.perform(function () {
    var orgin_fgets = Module.findExportByName('libc.so', 'fgets');
    var fake_fgets = new NativeFunction(orgin_fgets, 'pointer', ['pointer', 'int', 'pointer']);
    Interceptor.replace(orgin_fgets, 
    new NativeCallback(function (buffer, size, fp) 
    {
        var retval = fake_fgets(buffer, size, fp);
        var buf = Memory.readUtf8String(buffer);
        if (buf.indexOf('TracerPid:') != -1) 
        {
            Memory.writeUtf8String(buffer, 'TracerPid:\t0');
            console.log('Tracerpid has been replaced: ' + Memory.readUtf8String(buffer));
        }
        return retval;
    }, 
    'pointer', ['pointer', 'int', 'pointer']));
});
"""

# 远程RPC服务
jsRpcCode = """
function sum(args)
{
    console.log("Call func with args: " + args[0], args[1])
    var base_address = Module.findBaseAddress("libdebug.so");
    if (base_address) 
    {
        var func_offset = 0x1F17C;  
        var func_addr = base_address.add(func_offset);
        // 将指针转换为一个可以调用的函数
        var add = new NativeFunction(func_addr, "int", ["int", "int"]);
        
        return add(args[0], args[1]);
    }
}
// 将函数注册暴露
rpc.exports.callrpcfunction=sum
"""

# 函数替换
jsFakeMethod = """
Java.perform(function () {
    var base_address = Module.findBaseAddress("libdebug.so")
    if (base_address) {
        var func_offset = 0x1F17C;  
        var orgin_func = base_address.add(func_offset)
        console.log("The address of func: " + orgin_func);
        // 自定义函数
        var fake_func = new NativeCallback(function(a,b)
        {
            console.log("Called with arguments: ", a, b);
            return a + b+20;
        }, 'int', ['int', 'int']);
        
        Interceptor.replace(orgin_func, fake_func);
    }
});
"""

# 遍历导出函数
jsScanExportCode = """
Java.perform(function(){
    var nativeLib = Process.findModuleByName("libdebug.so");
    var exports = nativeLib.enumerateExports();
    exports.forEach(function(exp){
        if(exp.name.indexOf('Java') != -1){
            console.log(exp.name + ":" + exp.address);
        }
        
    });
});
"""

# Hook非导出函数
jsHookUnexportedCode = """
Java.perform(function(){
    var base_address = Module.findBaseAddress("libdebug.so")
    console.log("Base address of libdebug.so: " + base_address);
    if (base_address) {
        // 目标函数在SO中的偏移底止
        var func_offset = 0x1F17C;  
        // 内存中函数地址 = SO基地址 + 函数偏移
        var addr_func = base_address.add(func_offset)
        
        console.log("The address of func: " + addr_func);
    
        var pfunc = new NativePointer(addr_func);
        Interceptor.attach(pfunc, {
            onEnter: function (args) {
                console.log("Call func with args: " + args[0], args[1])
            },
            onLeave: function (retval) {
                console.log("func return:", retval)
            }
        })
        
    }
});
"""

# Hook导出函数并修改返回值
jsHookFunc = """
Java.perform(function(){
    var nativeLib = Process.findModuleByName("libdebug.so");
    var addr = nativeLib.findExportByName("Java_com_dynamic_debug_MainActivity_checkInput");
    Interceptor.attach(addr, {
        onEnter: function(args) {
            // JNI 函数传入的前两个参数是JENv和jobject,第三个才是传入的参数。
            var arg = Java.vm.getEnv().getStringUtfChars(args[2], null).readCString()
            console.log("Call func with args:", arg);
        },
        onLeave: function(retval) {
            console.log("Func return: ", retval);
            retval.replace(0x1);
        }
    });
});
"""
def printMessage(message,data):
    if message['type'] == 'send':
        print(' {0}'.format(message['payload']))
    else:
        print(message)

device = frida.get_usb_device()
pid = device.spawn(["com.dynamic.debug"])
device.resume(pid)
session = device.attach(pid)
time.sleep(3)
script= session.create_script(jsRpcCode)
script.on('message',printMessage)
script.load()
# sys.stdin.read()


ret = script.exports.callrpcfunction([1,2])
print("Call rpc result:",ret)

