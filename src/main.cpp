#include <Gun.h>

#include <twi.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>


#define I2C_ADDR    0x27 // <<----- Add your address here.  Find it from I2C Scanner
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

int n = 1;

LiquidCrystal_I2C lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);

Gun gun;

void printAmmo() {
    // print ammo to lcd

    int bullets = gun.getBulletsInMag();

    lcd.setCursor (5, 1);

    if(bullets < 10) {
        lcd.print(" ");
    }

    lcd.print(bullets, DEC);
    lcd.setCursor (7, 1);
    lcd.print("/");
    lcd.setCursor (8, 1);
    lcd.print(gun.getMaxAmmoInMag(), DEC);
}

void printReloadingState() {

    // @TODO: should be optimized to only clear the
    // chars if the last state was reloading

    lcd.setCursor (0, 4);
    if(gun.isReloading()) {
        lcd.print("Reloading..");
    } else {
        lcd.print("           ");
    }
}

void setup() {
    lcd.begin(20, 4); // 20 chars, 4 lines

    // Switch on the backlight
    lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
    lcd.setBacklight(HIGH);
    lcd.home(); // go home

    lcd.print("#laserTag");
    lcd.setCursor (0, 1);
    lcd.print("Mag: ");
}

void loop() {
    lcd.setBacklight(HIGH);

    gun.tick();
    printAmmo();
    printReloadingState();
}

