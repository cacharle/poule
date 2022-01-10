# Poule

> poule means chicken in french, it sound like pool, I am a very funny person.

Educational project to understand of thread pools work.

## Usage

Install the [meson build system][1]

```
$ meson setup build
$ ninja -C build
```

Run one of the examples:

```
$ ./build/examples/map
$ ./build/examples/pi
```

## TODO

- [ ] Process pool
- [ ] Benchmark
- Multiple examples
     - [ ] Mandelbrot
     - [x] Approximate Pi
- [x] Make a library
- [ ] Add `pool_map_unordered`
- Unit test
     - [ ] `queue`
     - [ ] `pool`
- [ ] Move the function parameter to `submit` instead of `pool_init`
- [ ] Add optional callback
- [ ] Add batch system for computation which take little time.
      Wrap the user function in a loop (in `worker_routine`)



[1]: https://mesonbuild.com/index.html
