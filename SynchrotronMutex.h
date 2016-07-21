/**
 *	Loosely based on https://github.com/winglot/Signals
 */
#ifndef SYNCHROTRONMUTEX_H
#define SYNCHROTRONMUTEX_H

#include <mutex>

namespace Synchrotron {
	class Mutex {
		private:
			std::mutex m_mutex;
		public:
			Mutex() {}
			Mutex(const Mutex&) {}
			virtual ~Mutex() {}

			virtual void lock() {
				m_mutex.lock();
			}

			virtual void unlock() {
				m_mutex.unlock();
			}
	};

	class LockBlock {
		public:
			Mutex *m_mutex;

			LockBlock(Mutex *mtx): m_mutex(mtx) {
				m_mutex->lock();
			}

			~LockBlock() {
				m_mutex->unlock();
			}
	};
}

#endif // SYNCHROTRONMUTEX_H
