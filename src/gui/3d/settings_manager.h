#ifndef SETTINGS_MANAGER_H
#define SETTINGS_MANAGER_H
// pattern Singleton

#include <QSettings>
namespace s21 {
class Settings_manager {
 public:
  static Settings_manager &instance() {
    static Settings_manager instance;
    return instance;
  }
  QSettings &settings() { return settings_; }

 private:
  Settings_manager() : settings_("s21", "3DViewer") {}
  ~Settings_manager() = default;

  QSettings settings_;

  Settings_manager(const Settings_manager &) = delete;
  Settings_manager &operator=(const Settings_manager &) = delete;
};
}  // namespace s21

#endif  // SETTINGS_MANAGER