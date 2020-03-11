data "template_file" "ssh_config" {
  template = file("${path.module}/ssh_config.in")
  vars     = {
    host_address = hcloud_server.main.ipv4_address
    admin_user   = var.admin_user
  }
}

output "ssh_config" {
  value = data.template_file.ssh_config.rendered
}
