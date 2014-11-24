/**
* Lasertag Gun Class
*/
#include "Arduino.h"
#include "IRremote.h"
#include <gun_config.h>

class Gun {
	public:
		Gun();
		void tick();
        int getBulletsInMag();
        int getMaxAmmoInMag();
        bool isReloading();
	private:
        static const int STATE_RELOADING = 1;
        static const int STATE_READY_TO_FIRE = 2;
        
		int _currentAmmoInMag;
		int _buttonState;
		int _state;
		unsigned long _time;
		unsigned long _timeNextBulletInChamber;
		unsigned long _reloadUntil;
		
		IRsend _irsend;

        void checkReload();
        void checkHitIndicator();
        bool shoot();
        bool checkBulletInChamber();
        void reload();
        void hitIndicatorOn();
        void hitIndicatorOff();
        void playSound(int type);
};