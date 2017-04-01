require(File.join(ENV['gubg'], 'shared'))
require('gubg/build/Library')
include GUBG

task :default => :help
task :help do
    puts("The following tasks can be specified:")
    puts("* declare: installs the arduino SDK")
    puts("* define: ")
end

task :clean do
    rm_rf '.cache'
    rm(FileList.new('lib*.a'))
end

task :declare do
    build_ok_fn = 'gubg.build.ok'

    Dir.chdir(shared_dir('extern')) do
        if false
            git_clone('https://github.com/arduino', 'Arduino') do
                if !File.exist?(build_ok_fn)
                    puts("Installing arduino")
                    # sh 'rm -rf build'
                    # sh "make -j 8 CMAKE_EXTRA_FLAGS=\"-DCMAKE_INSTALL_PREFIX:PATH=#{shared('install', 'neovim')}\""
                    # sh 'make install'
                    # Dir.chdir(shared('install', 'neovim', 'bin')) do
                    #     publish('nvim', dst: 'bin', mode: 0755)
                    # end
                    # sh "touch #{build_ok_fn}"
                end
            end
        else
            unless File.exist?('Arduino-master')
                zip_fn = 'master.zip'
                sh "wget https://github.com/arduino/Arduino/archive/#{zip_fn}" unless File.exist?(zip_fn)
                sh "unzip #{zip_fn}" 
            end
        end
    end
end

#sudo apt install gcc-avr avr-libc avrdude
task :define => :declare do
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
        lib.add_sources(File.join(lib_dir, "Servo/src/avr/Servo.cpp"))
        lib.build
        publish(lib.lib_filename, dst: "lib/#{arch}")
    end
end

task :test do
end
