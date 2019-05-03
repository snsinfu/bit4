data "template_file" "start_server_app" {
  template = "${file("start_server_app.bash.in")}"
  vars {
    hostname = "${local.hostname}"
    email    = "${var.email}"
  }
}

output "start_server_app" {
  value     = "${data.template_file.start_server_app.rendered}"
  sensitive = true
}
