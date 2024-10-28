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

## How it works

This program writes controller output to the `Local\SharedMemoryHidBuzzerReader` shared-memory file that can be read by other programs.

The output string looks like this:

```txt
RYGOBRYGOBRYGOBRYGOB
```

Each block of 5 characters represents a controller and its 5 colored buttons.

| Char position | Button | letter |
|---|---|---|
| 1 | Red | R |
| 2 | Yellow | Y |
| 3 | Green | G |
| 4 | Orange | O |
| 5 | Blue | B |

When a button is pressed, the letter associated with it is present in the string, otherwise it is replaced by `x`.

Examples:

- `xxxxxxxxxxxxxxxxxxxx` - No button pressed.
- `RxxxxxYxxxxxxxxxxxxx` - The **red** button on **controller 1** and the **yellow** button on **controller 2** are pressed.
- `xxxxxRYxxxxxGxxxxxxx` - The **red** and **yellow** buttons on **controller 2** and the **green** button on **controller 3** are pressed.
- `RYGOBRYGOBRYGOBRYGOB` - All buttons are pressed.

## Development

### Dependencies

- [libusb/hidapi](https://github.com/libusb/hidapi)
  - Install library files in `libs` directory
  - Install header files in `includes` directory

### To Do

- [x] Develop basic reader program
- [ ] Develop basic writer program
- [ ] Use multithreading to combine both reader and writter programs
- [ ] Optimize code
- [ ] Support Linux
