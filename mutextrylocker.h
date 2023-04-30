#ifndef MUTEXTRYLOCKER_H
#define MUTEXTRYLOCKER_H
#include <QMutex>


class MutexTryLocker {
  QMutex &m_;
  bool locked_;
public:
  /*!
   * \brief MutexTryLocker constructor
   * \param m
   */
  MutexTryLocker(QMutex &m) : m_(m), locked_(m.tryLock()) {}

  ~MutexTryLocker() { if (locked_) m_.unlock(); }

  /*!
   * \brief isLocked
   * \return true if mutex is locked and false otherwise
   */
  bool isLocked() const { return locked_; }
};

#endif // MUTEXTRYLOCKER_H
