require 'dotenv'
require 'fog'
require 'fog/scaleway'

Dotenv.load

compute = Fog::Compute.new({
  provider: :scaleway,
  scaleway_organization: ENV['SCW_ORGANIZATION'],
  scaleway_token: ENV['SCW_TOKEN'],
  scaleway_region: ENV['SCW_REGION']
})

server = compute.servers.create({
  name: 'test',
  commercial_type: 'ARM64-2GB',
  image: 'e1a0daf2-a61f-40bf-ba3c-f86ec4baa3bf', # arm64 Ubuntu Xenial
  bootscript: 'b73576d3-d45a-4d3a-9a89-087c32e4cee3', # aarch64 mainline 4.14.14 rev1
})

begin
  server.poweron(async: false)
  server.wait_for { server.sshable? }
  puts server.ssh('uname -r').first.stdout
ensure
  server.terminate(async: false)
  server.destroy
end
