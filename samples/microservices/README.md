# Microservices

The goal of this demo is to show that using RPCs is not just confined to remote calls. Instead, when a module is needed
to be flexible, forward maintainable, and portable; an RPC can and should be considered with minimal overhead. To
run the profiling version of this demo in `native_posix`, use [profile.sh](profile.sh). To run it on device, use:

```bash
$ west build -p -b ${board} samples/microservices && west flash
```
