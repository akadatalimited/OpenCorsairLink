# OpenCorsairLink 2025

A modernized continuation of the original [OpenCorsairLink](https://github.com/audiohacked/OpenCorsairLink) project by **Audiohacked**, now maintained by [akadata.ltd](https://akadata.ltd), focused on bringing full support for Corsair HXi and RMi series power supplies to **modern Linux systems** via USB HID.

---

## ✨ Acknowledgements

> All credit and gratitude to **Audiohacked**, the original creator and developer of OpenCorsairLink.
> This fork exists solely because of the foundational reverse engineering, C code, and community support that Audiohacked provided. The original project remains archived [here](https://github.com/audiohacked/OpenCorsairLink).

Audiohacked's retirement note:

> I (Audiohacked) have decided to retire OpenCorsairLink. \[...] As I am still a fan of Corsair Products, any work/development/reverse-engineering I do in the future will be available at [liquidctl](https://github.com/jonasmalacofilho/liquidctl).

We honor that contribution and continue in that spirit.

---

## 🔄 This Fork

This 2025 fork brings:

* Full support for **modern distros (Ubuntu 24.04+, Arch, etc.)**
* Modernized **Makefile**, compile fixes, and libusb support
* Supports **HX850i, RM850i**, and likely other Link-based Corsair PSUs
* USB HID-based **telemetry**: voltages, current, power, temps
* Added `/dev/hidraw*` compatibility on Linux
* **No need for iCUE, Windows, or GUI bloat**
* Light and scriptable (bash, cron, Prometheus, etc.)

Upcoming features in progress:

* `--status` human-readable output
* `--json` for machine parsing and dashboards
* Systemd service + export options

---

## ⚙ Installation

```bash
git clone https://github.com/akadatalimited/OpenCorsairLink.git
cd OpenCorsairLink
make clean && make
sudo make install
```

Then run:

```bash
sudo corsairlink --device 0
```

---

## 📊 Example Output

```
Vendor: CORSAIR
Product: HX850i
Temperature 0: 45.04 C
Temperature 1: 39.66 C
Supply Voltage: 230.00 V
Total Watts: 68.00 W
Output 12v: 12.09 V / 5.00 A / 56.00 W
Output 5v:   5.03 V / 1.88 A / 9.00 W
Output 3.3v: 3.31 V / 1.50 A / 5.00 W
```

---

## 🌐 Repository Management

This repository is no longer a fork of the original and is maintained independently.

* Default branch: `master`
* Tags will be published as: `v2025.x`
* Development happens in topic branches

If you're using this project and want to contribute, open an issue or pull request.

---

## 🚀 Why?

Because Linux users deserve:

* To see what their PSU is doing
* Without proprietary software
* Without Windows
* Without guesswork

This project is for builders, sysadmins, and clean system lovers.

---

## ✈ License

GPLv2+ (as per original project).

---

**Maintained by [akadata.ltd](https://akadata.ltd)** — with respect to the Creator and Creator of all things.
