# Buzz Driver

Program to drive those little Buzz controllers.

```txt
/{🔴}\  /{🔴}\  /{🔴}\  /{🔴}\
| 🟦 |  | 🟦 |  | 🟦 |  | 🟦 |
| 🟧 |  | 🟧 |  | 🟧 |  | 🟧 |
| 🟩 |  | 🟩 |  | 🟩 |  | 🟩 |
| 🟨 |  | 🟨 |  | 🟨 |  | 🟨 |
\_🔹_/  \_🔹_/  \_🔹_/  \_🔹_/
```

## Dependencies

- [libusb/hidapi](https://github.com/libusb/hidapi)
  - Install library files in `libs` directory
  - Install header files in `includes` directory

## To Do

- [x] Develop basic reader program
- [ ] Develop basic writer program
- [ ] Use multithreading to combine both reader and writter programs
- [ ] Optimize code
- [ ] Support Linux
