resource "hcloud_server" "master" {
  name        = var.server_hostname
  server_type = var.server_type
  location    = var.server_location
  image       = data.hcloud_image.freebsd.id
  user_data   = data.template_file.init_script.rendered
}

data "template_file" "init_script" {
  template = file("${path.module}/init_script.sh.in")
  vars     = {
    admin_user    = var.admin_user
    admin_passwd  = bcrypt(var.admin_password)
    admin_pubkeys = join("\n", var.admin_pubkeys)
  }
}

data "hcloud_image" "freebsd" {
  with_selector = "custom_image=freebsd-12.1"
}
