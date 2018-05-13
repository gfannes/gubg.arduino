require("gubg/shared")

module Arduino
    def self.program(exe_fn, na = {arch: nil})
        exe_fn = File.absolute_path(exe_fn)
        puts(">> Programming #{exe_fn}")

        GUBG::sandbox(chdir: false) do |dir|
            eep_fn = File.join(dir, "output.eep")
            hex_fn = File.join(dir, "output.hex")
            Rake.sh "avr-objcopy -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 #{exe_fn} #{eep_fn}"
            Rake.sh "avr-objcopy -O ihex -R .eeprom #{exe_fn} #{hex_fn}"

            arch = (na[:arch]||:uno).to_sym
            settings = {
                mega:    {programmer: "wiring", partno: "atmega2560", baudrate: 115200},
                lilypad: {programmer: "avr109", partno: "atmega32u4", baudrate: 57600},
            }[arch]
            raise("Could not find avrdude settings for arch \"#{arch}\"") unless settings

            programmer, partno, baudrate = *[:programmer, :partno, :baudrate].map{|sym|settings[sym]}
            tty_fn = "/dev/ttyACM0"
            raise("TTY #{tty_fn} does not exist") unless File.exist?(tty_fn)

            Rake.sh "avrdude -C//etc/avrdude.conf -v -p#{partno} -c#{programmer} -P#{tty_fn} -b#{baudrate} -D -Uflash:w:#{hex_fn}:i"
        end

        puts("<< Programming #{exe_fn}")
    end
end
