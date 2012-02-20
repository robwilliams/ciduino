require 'serialport'
require 'pusher-client'
require 'json'

PusherClient.logger = Logger.new(STDOUT)

socket = PusherClient::Socket.new(ENV['APP_KEY'])

mappings = {
  "red"    => {"fade" => 7, "flash" => 4, "on" => 1},
  "yellow" => {"fade" => 8, "flash" => 5, "on" => 2},
  "green"  => {"fade" => 9, "flash" => 6, "on" => 3}
}

sp = SerialPort.new("/dev/tty.usbmodemfa131", 9600)
sleep(1) # wait for serial connection

socket.subscribe('lights')

socket['lights'].bind('change') do |data|
  Process.kill("HUP", @pid) unless @pid.nil?
  @pid = fork do
    Signal.trap("HUP") { puts "A new message! How exciting!"; exit }
    loop do
      JSON.parse(data).each do |colour, level|
        sp.write(mappings[colour][level])
        sleep(1)
      end
    end
  end
end

trap("SIGINT") { puts "Tidying up"; Process.kill("HUP", @pid) unless @pid.nil?; exit }

socket.connect
sp.close # close serial connection