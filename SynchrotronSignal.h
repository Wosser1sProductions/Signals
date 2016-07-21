/**
 *	Loosely based on https://github.com/winglot/Signals
 */
#ifndef SYNCHROTRONSIGNAL_H
#define SYNCHROTRONSIGNAL_H

#include <forward_list>
#include "SynchrotronMutex.h"
#include "SynchrotronSlot.h"

using namespace std;

namespace Synchrotron {

	/**
	 *	[Subscriber pattern :: Signals&Slots]
	 *
	 *	Publisher/Signal class
	 *		Signal is used as a synchronous clock, with each tick() giving a notification to all subscribers
	 *		Others subscribe to classes implementing Signal to receive updates.
	 */
	class Signal : public SignalBase {
		private:
		protected:
			std::forward_list<Slot*> slots;
		public:
			/**
             * Construct a new Signal
             */
			Signal() {
				// Empty
			}

            /**
             * Copy constructor
             */
            Signal(const Signal& s) {
            	LockBlock lock(this);

            	for(auto& connection : s.slots) {
					connection->signal_connect(this);
					this->slots.push_front(connection);
            	}
            }

			~Signal() {
				this->disconnect_all_slots();
			}

			void connectSlot(Slot* s) {
				LockBlock lock(this);

				this->slots.push_front(s);
				s->signal_connect(this);
			}

			void disconnectSlot(Slot* s) {
				LockBlock lock(this);

				this->slots.remove(s);
				s->signal_disconnect(this);
			}

			void slot_disconnect(Slot* s) {
				LockBlock lock(this);

				this->slots.remove(s);
			}

			void slot_duplicate(Slot* s) {
				LockBlock lock(this);

				this->slots.push_front(s);
			}

            void disconnect_all_slots() {
                LockBlock lock(this);

                for(auto& connection : this->slots) {
					connection->signal_disconnect(this);
                    delete connection; //?
                }

                //this->slots.erase(this->slots.begin(), this->slots.end());
				this->slots.clear();
            }

            /**
             * Emit this signal and execute all subscribed slots
             */
            void emit() {
                LockBlock lock(this);

				for(auto& connection : this->slots) {
					connection->tick();
				}
            }
	};
}

#endif // SYNCHROTRONSIGNAL_H
