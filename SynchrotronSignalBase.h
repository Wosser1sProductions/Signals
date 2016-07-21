/**
 *	Loosely based on https://github.com/winglot/Signals
 */
#ifndef SYNCHROTRONSIGNALBASE_H
#define SYNCHROTRONSIGNALBASE_H

#include "SynchrotronMutex.h"

namespace Synchrotron {

	class Slot;

	class SignalBase : public Mutex {
		public:
			virtual ~SignalBase() {}
			virtual void slot_duplicate(Slot *s) = 0;
			virtual void slot_disconnect(Slot *s) = 0;
	};

}

#endif // SYNCHROTRONSIGNALBASE_H
