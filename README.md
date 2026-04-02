# Zephyr Training Environment

Welcome to the my Zephyr "Sandbox" training project for the ZephyrRTOS course of iomico.

This project is currently configured (as shown in the west.yml manifest) for zephyr v4.4.0-rc1, because one of the boards I want to use has almost all support added between v4.3.0 (latest "release" version) and current mainline. When v4.4.0 is released on april 2026, I will lock it for the duration of the project.

---

## Manual Zephyr Setup

Follow the Zephyr guide:
- [Getting Started Guide](https://docs.zephyrproject.org/latest/develop/getting_started/index.html#).

---
## Build & Flash
You can use the clasic `west build <args>` for your board.
Currently there is no "change" against the dts for the boards, so, it should work on any Zephyr-Supported board.
There was added a /scripts folder, where I will be adding usefull scripts (To not needing remembering all commands).

---