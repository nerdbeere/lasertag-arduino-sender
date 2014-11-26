#include <Gun.h>

#include <twi.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <Health.h>

#define I2C_ADDR    0x27 // <<----- Add your address here.  Find it from I2C Scanner
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

int TYPE_HIT = 1;
int lastGunState = 0;

LiquidCrystal_I2C lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);

Gun gun;
Health health;

void printAmmo() {
    // print ammo to lcd

    int bullets = gun.getBulletsInMag();

    lcd.setCursor(5, 0);

    if (bullets < 10) {
        lcd.print(" ");
    }

    lcd.print(bullets, DEC);
    lcd.setCursor(7, 0);
    lcd.print("/");
    lcd.setCursor(8, 0);
    lcd.print(gun.getMaxAmmoInMag(), DEC);
}

/**
* Prints the current hitpoints of the player.
* Warning: The maximum HP can't be over 999
*/
void printHealth() {
    lcd.setCursor(17, 0);
    int hp = health.getHealth();

    if(hp < 10) {
        lcd.print("  ");
        lcd.print(hp);
        return;
    }

    if(hp < 100) {
        lcd.print(" ");
        lcd.print(hp);
        return;
    }

    lcd.print(hp);
}

void printReloadingState() {

    // if the state didn't change we don't do anything
    if(lastGunState == gun.getCurrentState()) {
        return;
    }

    lcd.setCursor(0, 4);

    // is the gun reloading?
    if (gun.isReloading()) {
        lcd.print("Reloading..");
        return;
    }

    // if the last state was reloading and the we switched to ready the we clear the display
    if(gun.getLastState() == gun.STATE_RELOADING && gun.getCurrentState() == gun.STATE_READY_TO_FIRE) {
        lcd.print("           ");
    }
}

void setupLcd() {
    lcd.begin(20, 4); // 20 chars, 4 lines

    // Switch on the backlight
    lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
    lcd.setBacklight(HIGH);
    lcd.home(); // go home

    lcd.print("Mag:");

    lcd.setCursor(13, 0);
    lcd.print("HP:");
}

void receiveEvent(int howMany) {
    int type = Wire.read();
    if(type == TYPE_HIT) {
        int playerId = Wire.read();
        int teamId = Wire.read();
        int damage = Wire.read();
        int critical = Wire.read();
        health.takeDamage(playerId, teamId, damage, critical);
        return;
    }
}

void setup() {

    Serial.begin(9600);
    Wire.begin(1);
    Wire.onReceive(receiveEvent);

    setupLcd();
}

void loop() {
    gun.tick();

    printAmmo();
    printReloadingState();
    printHealth();

    lastGunState = gun.getCurrentState();
}

