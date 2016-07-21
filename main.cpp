#include <iostream>
using namespace std;

#include "SynchrotronSignal.h"
#include "SynchrotronSlot.h"

using namespace Synchrotron;

class Subscriber : public Slot {
	private:
		std::string s;
	public:
		Subscriber(std::string str) : s(str) {}

		void tick() {
			cout << "Hello " << this->s << "!" << endl;
		}
};

class Publisher : public Signal {
	public:
		Publisher() : Signal() {}
};



int main_old() {

	Publisher p;
	Subscriber s1("test 1"), s2("test 2");

	p.connectSlot(&s1);
	p.connectSlot(&s2);

	p.emit();

	cout << endl << endl;

	p.disconnectSlot(&s1);
	p.emit();

	cout << endl << endl;

	{
		Subscriber s3("test 3");
		p.connectSlot(&s3);
		p.emit();
	}

	cout << endl << endl;
	p.emit();


    return 0;
}
