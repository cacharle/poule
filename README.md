# Poule

> poule means chicken in french, it sound like pool, I am a very funny person.

Educational project to understand of thread pools work.

## Usage

Install the [meson build system][1]

```
$ meson setup build
$ ninja -C build
$ ./build/example
```

## TODO

- [ ] Process pool
- [ ] Benchmark
- Multiple examples
     - [ ] Mandelbrot
     - [ ] Approximate Pi
- [x] Make a library
- [ ] Add `pool_map_unordered`
- Unit test
     - [ ] `queue`
     - [ ] `pool`
- [ ] Move the function parameter to `submit` instead of `pool_init`

[1]: https://mesonbuild.com/index.html
