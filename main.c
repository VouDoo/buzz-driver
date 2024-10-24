#include "includes/hidapi.h"
#include <stdbool.h>
#include <stdio.h>
#include <windows.h>

#define MAX_STR 255

// HID definitions
#define HID_VID 0x054C  // USB Vendor ID
#define HID_PID 0x1000  // USB Product ID
#define HID_READ_REPORT_SIZE 5
#define HID_WRITE_REPORT_SIZE 6

// Button symbol definitions
#define BTN_SYMBOL_UNSET 'X'
#define BTN_SYMBOL_RED 'R'
#define BTN_SYMBOL_YELLOW 'Y'
#define BTN_SYMBOL_GREEN 'G'
#define BTN_SYMBOL_ORANGE 'O'
#define BTN_SYMBOL_BLUE 'B'

// Buffer size for shared memory
#define BUFFER_SIZE 21 // 20 buttons + null-terminator
const char *sMemReader = "Local\\SharedMemoryHidBuzzerReader";

struct {
  // byte index in report related to the button
  int byte_idx;
  // bitmask in byte to be checked
  int bitmask;
  // symbol that represents the button
  char symbol;
} btn_map[] = {
    // Controller 1
    {2, 0x01, BTN_SYMBOL_RED},
    {2, 0x02, BTN_SYMBOL_YELLOW},
    {2, 0x04, BTN_SYMBOL_GREEN},
    {2, 0x08, BTN_SYMBOL_ORANGE},
    {2, 0x10, BTN_SYMBOL_BLUE},
    // Controller 2
    {2, 0x20, BTN_SYMBOL_RED},
    {2, 0x40, BTN_SYMBOL_YELLOW},
    {2, 0x80, BTN_SYMBOL_GREEN},
    {3, 0x01, BTN_SYMBOL_ORANGE},
    {3, 0x02, BTN_SYMBOL_BLUE},
    // Controller 3
    {3, 0x04, BTN_SYMBOL_RED},
    {3, 0x08, BTN_SYMBOL_YELLOW},
    {3, 0x10, BTN_SYMBOL_GREEN},
    {3, 0x20, BTN_SYMBOL_ORANGE},
    {3, 0x40, BTN_SYMBOL_BLUE},
    // Controller 4
    {3, 0x80, BTN_SYMBOL_RED},
    {4, 0xF1, BTN_SYMBOL_YELLOW},
    {4, 0xF2, BTN_SYMBOL_GREEN},
    {4, 0xF4, BTN_SYMBOL_ORANGE},
    {4, 0xF8, BTN_SYMBOL_BLUE}};

int turn_on_leds(hid_device *handle, bool ctrl_1, bool ctrl_2, bool ctrl_3,
                 bool ctrl_4) {
  int res;
  unsigned char buf[HID_WRITE_REPORT_SIZE] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

  // turn on LEDs if requested by controllers
  if (ctrl_1)
    buf[2] = 0xFF;
  if (ctrl_2)
    buf[3] = 0xFF;
  if (ctrl_3)
    buf[4] = 0xFF;
  if (ctrl_4)
    buf[5] = 0xFF;

  // write report to device
  res = hid_write(handle, buf, HID_WRITE_REPORT_SIZE);

  return res;
}

int read_input(hid_device *handle, char *msg) {
  unsigned char buf[HID_READ_REPORT_SIZE];
  int res;

  // read report from device
  res = hid_read(handle, buf, HID_READ_REPORT_SIZE);

  // check buttons that are pressed
  for (int i = 0; i < 20; i++) {
    if ((buf[btn_map[i].byte_idx] & btn_map[i].bitmask) == btn_map[i].bitmask) {
      msg[i] = btn_map[i].symbol;
    } else {
      msg[i] = BTN_SYMBOL_UNSET;
    }
  }
  // terminate string
  msg[BUFFER_SIZE - 1] = '\0';

  return res;
}

int main(int argc, char *argv[]) {
  hid_device *handle = NULL;
  wchar_t wstr[MAX_STR];
  char msg[BUFFER_SIZE];
  int res;

  // create a file mapping object for shared memory
  HANDLE hMapFile = CreateFileMapping(
      INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, BUFFER_SIZE, sMemReader);
  if (hMapFile == NULL) {
    fprintf(stderr, "Could not create file mapping object.\n");
    return 1;
  }
  char *pBuf =
      (char *)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, BUFFER_SIZE);

  // initialize hidapi
  hid_init();
  handle = hid_open(HID_VID, HID_PID, NULL);
  if (handle) {
    printf("Connected to device successfully.\n");
  } else {
    fprintf(stderr, "Unable to open device.\n");
    hid_exit();
    return 1;
  }

  // set non-blocking mode
  hid_set_nonblocking(handle, false);

  // print information about the device
  hid_get_manufacturer_string(handle, wstr, MAX_STR);
  printf("Manufacturer String: %ls\n", wstr);
  hid_get_product_string(handle, wstr, MAX_STR);
  printf("Product String: %ls\n", wstr);

  // turn on all LEDs
  turn_on_leds(handle, true, true, true, true);

  // read input from device
  while (true) {
    res = read_input(handle, msg);
    if (res >= 0) {
      // write message to shared memory
      sprintf(pBuf, "%s", msg);
    } else {
      fprintf(stderr, "Failed to read from device.\n");
      break;
    }
  }

  // close device and exit
  hid_close(handle);
  hid_exit();

  return 0;
}