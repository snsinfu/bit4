output "ssh_config" {
  value = data.template_file.ssh_config.rendered
}

data "template_file" "ssh_config" {
  template = file("${path.module}/ssh_config.in")
  vars = {
    admin_user     = var.admin_user
    server_address = hcloud_server.main.ipv4_address
  }
}
