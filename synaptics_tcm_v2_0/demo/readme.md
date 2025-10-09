touch_grab.c
/media/work/work/rk_sdk/rk_linux_sdk/buildroot/output/rockchip_rk3566/host/bin/aarch64-linux-gcc -O2 -o touch_grab touch_grab.c
adb push touch_grab /userdata/
adb shell
cd /userdata
chmod +x touch_grab
# 以 root 运行
./touch_grab


touch_latency.c
/media/work/work/rk_sdk/rk_linux_sdk/buildroot/output/rockchip_rk3566/host/bin/aarch64-linux-gcc -O2 -o touch_latency touch_latency.c
adb push touch_latency /userdata
adb shell
cd /userdata
chmod +x touch_latency
./touch_latency -n 1000

