require(File.join(ENV['gubg'], 'shared'))
require('gubg/build/Library')
include GUBG

task :default do
    sh "rake -T"
end

task :clean do
    rm_rf '.cache'
    rm(FileList.new('lib*.a'))
end

task :prepare do
    GUBG::publish('src', pattern: '**/*.rb', dst: 'ruby')
    Dir.chdir(GUBG::mkdir('extern')) do
        unless File.exist?('ArduinoCore-avr')
            sh "git clone --depth 1 https://github.com/arduino/ArduinoCore-avr"
        end
        unless File.exist?('Servo')
            sh "git clone --depth 1 https://github.com/arduino-libraries/Servo"
        end
    end
end

#sudo apt install gcc-avr avr-libc avrdude
task :run do
    archs = [:lilypad, :uno]
    # archs = [:uno]
    archs.each do |arch|
        lib = Build::Library.new('arduino-core', arch: arch)
        puts("#{lib.lib_filename}")
        avr_dir = shared('extern/Arduino-master/hardware/arduino/avr')
        cpp_parts = %w[main]
        c_parts = %w[wiring hooks wiring_digital]
        case arch
        when :lilypad
            cpp_parts += %w[USBCore PluggableUSB CDC HardwareSerial HardwareSerial0 HardwareSerial1 HardwareSerial2 HardwareSerial3 IPAddress Print Stream Tone WMath WString abi new] 
            c_parts += %w[WInterrupts wiring_analog wiring_pulse wiring_shift]
        end
        lib.add_sources(cpp_parts.map{|part|File.join(avr_dir, "cores/arduino/#{part}.cpp")})
        lib.add_sources(c_parts.map{|part|File.join(avr_dir, "cores/arduino/#{part}.c")})
        lib_dir = shared('extern/Arduino-master/libraries')
        servo_cpp = File.join(lib_dir, "Servo/src/avr/Servo.cpp")
        lib.add_sources(servo_cpp) if File.exist?(servo_cpp)
        lib.build
        publish(lib.lib_filename, dst: "lib/#{arch}")
    end
end

task :test do
end
