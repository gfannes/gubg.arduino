require_relative("../gubg.build/bootstrap.rb")
require("gubg/shared")

task :default do
    sh "rake -T"
end

task :clean do
    rm_rf '.cache'
    rm(FileList.new('lib*.a'))
end
task :proper do
    rm_rf "extern"
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
