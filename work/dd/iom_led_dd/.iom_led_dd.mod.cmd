cmd_/home/leeejjju/bbbb/work/dd/iom_led_dd/iom_led_dd.mod := printf '%s\n'   iom_led_dd.o | awk '!x[$$0]++ { print("/home/leeejjju/bbbb/work/dd/iom_led_dd/"$$0) }' > /home/leeejjju/bbbb/work/dd/iom_led_dd/iom_led_dd.mod