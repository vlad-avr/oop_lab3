#ifndef MUTEXTRYLOCKER_H
#define MUTEXTRYLOCKER_H
#include <QMutex>


class MutexTryLocker {
  QMutex &m_;
  bool locked_;
public:
  MutexTryLocker(QMutex &m) : m_(m), locked_(m.tryLock()) {}
  ~MutexTryLocker() { if (locked_) m_.unlock(); }
  bool isLocked() const { return locked_; }
};

#endif // MUTEXTRYLOCKER_H
