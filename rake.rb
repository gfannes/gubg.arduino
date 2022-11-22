require("gubg/shared")

here = File.dirname(__FILE__)

namespace :arduino do
	task :prepare do
		Dir.chdir(Gubg::mkdir(here, 'extern')) do
			unless File.exist?('ArduinoCore-avr')
				sh "git clone --depth 1 https://github.com/arduino/ArduinoCore-avr"
			end
			unless File.exist?('Servo')
				sh "git clone --depth 1 https://github.com/arduino-libraries/Servo"
			end
		end
	end
end