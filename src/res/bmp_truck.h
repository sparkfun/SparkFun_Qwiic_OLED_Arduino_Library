/****************************************************************************** 
font5x7.h
Definition for small font

This file was imported from the MicroView library, written by GeekAmmo
(https://github.com/geekammo/MicroView-Arduino-Library), and released under 
the terms of the GNU General Public License as published by the Free Software 
Foundation, either version 3 of the License, or (at your option) any later 
version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

Modified by:
Emil Varughese @ Edwin Robotics Pvt. Ltd.
July 27, 2015
https://github.com/emil01/SparkFun_Micro_OLED_Arduino_Library/

******************************************************************************/
#pragma once 

/*
#if defined(ARDUINO_ARCH_MBED)
	// ARDUINO_ARCH_MBED (APOLLO3 v2) does not support or require pgmspace.h / PROGMEM
	static const uint8_t bmp_truck[] = {
#elif defined(__AVR__) || defined(__arm__) || defined(__ARDUINO_ARC__)
	#include <avr/pgmspace.h>
	static const uint8_t bmp_truck[] PROGMEM = {
#else
	#include <pgmspace.h>
	static const uint8_t bmp_truck[] PROGMEM = {
#endif
*/

#define BMP_TRUCK_WIDTH  19
#define BMP_TRUCK_HEIGHT 16

static const uint8_t bmp_truck_data[] = {
    0xFF,
    0x01,
    0xC1,
    0x41,
    0x41,
    0x41,
    0x71,
    0x11,
    0x11,
    0x11,
    0x11,
    0x11,
    0x71,
    0x41,
    0x41,
    0xC1,
    0x81,
    0x01,
    0xFF,
    0xFF,
    0x80,
    0x83,
    0x82,
    0x86,
    0x8F,
    0x8F,
    0x86,
    0x82,
    0x82,
    0x82,
    0x86,
    0x8F,
    0x8F,
    0x86,
    0x83,
    0x81,
    0x80,
    0xFF,
};
