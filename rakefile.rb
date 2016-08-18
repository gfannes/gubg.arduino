require(File.join(ENV['gubg'], 'shared'))
require('gubg/build/Executable')
include GUBG

task :default => :help
task :help do
    puts("The following tasks can be specified:")
    puts("* declare: installs the arduino SDK")
    puts("* define: ")
end

task :clean do
    rm_rf '.cache'
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

task :define => :declare do
end

task :test do
end
