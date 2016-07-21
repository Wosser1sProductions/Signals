/**
 *	Loosely based on https://github.com/winglot/Signals
 */
#ifndef SYNCHROTRONSLOT_H
#define SYNCHROTRONSLOT_H

#include <set>
#include "SynchrotronMutex.h"

namespace Synchrotron {

	class SignalBase;

	/**
	 *	[Subscriber pattern :: Signals&Slots]
	 *
	 *	Subscriber/Slot class
	 *		Slot is a component needing synchronous updates on each tick of a clock.
	 *		It's subscribed to a Signal to receive updates.
	 */
	class Slot : public Mutex {
		protected:
			std::set<SignalBase*> connections;
		public:
			/**
             * Constructs a new Slot class
             */
            inline Slot();

            /**
             * Construct new Slot class from other Slot class
             * @param  hs Slot object to be copied
             */
            inline Slot(const Slot& hs);

            /**
             * Destructor releases all not released resources/signals
             */
            virtual inline ~Slot();

            /**
             * Connect given signal to slot
             * @param sender Signal to be connected
             */
            inline void signal_connect(SignalBase* sender);

            /**
             * Disconnect given signal from slot
             * @param sender Signal to be disconnected
             */
            inline void signal_disconnect(SignalBase* sender);

            /**
             * Disconnects all signals from this slot
             */
            inline void disconnect_all_signals();

			virtual void tick() = 0;
	};

}

/*
 * Implementation
 * Solves circular dependency problems without separate .cpp file
 */
#include "SynchrotronSlot.h"
#include "SynchrotronSignalBase.h"

namespace Synchrotron {

	Slot::Slot() {
		// Empty
	}

    Slot::Slot(const Slot& hs) : Mutex(hs) {
        LockBlock lock(this);

        for(auto& sender : hs.connections) {
            sender->slot_duplicate(this);
            this->connections.insert(sender);
        }
    }

    Slot::~Slot() {
        this->disconnect_all_signals();
    }

    void Slot::signal_connect(SignalBase* sender) {
        LockBlock lock(this);

        this->connections.insert(sender);
    }

    void Slot::signal_disconnect(SignalBase* sender) {
        LockBlock lock(this);

        this->connections.erase(sender);
    }


    void Slot::disconnect_all_signals() {
        LockBlock lock(this);

        for(auto &sender: this->connections) {
            sender->slot_disconnect(this);
        }

        //this->connections.erase(this->connections.begin(), this->connections.end());
        this->connections.clear(); // ?
    }

}

#endif // SYNCHROTRONSLOT_H
