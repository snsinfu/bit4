// Nginx pod on a single node cluster without load balancer (NodePort access).
//
// This configuration requires sl1pm4t's fork of the kubernetes provider plugin:
//
//      https://github.com/sl1pm4t/terraform-provider-kubernetes
//
// Build your copy and put into the vendor directory:
//
//      ./terraform.d/plugins/darwin_amd64/  (macOS)
//
// Also set GOOGLE_PROJECT environment variable.

provider "google" {
  zone = "us-central1-a"
}

resource "google_container_cluster" "main" {
  name               = "main"
  initial_node_count = 1

  master_auth {
    username = ""
    password = ""

    client_certificate_config {
      issue_client_certificate = true
    }
  }

  node_config {
    machine_type = "g1-small"
    preemptible  = true
    oauth_scopes = [
      "https://www.googleapis.com/auth/compute",
      "https://www.googleapis.com/auth/devstorage.read_only",
      "https://www.googleapis.com/auth/logging.write",
      "https://www.googleapis.com/auth/monitoring",
    ]
  }
}

locals {
  master_address         = "${google_container_cluster.main.endpoint}"
  client_key             = "${google_container_cluster.main.master_auth.0.client_key}"
  cluster_ca_certificate = "${google_container_cluster.main.master_auth.0.cluster_ca_certificate}"
}

provider "kubernetes" {
  host                   = "https://${local.master_address}"
  client_key             = "${base64decode(local.client_key)}"
  cluster_ca_certificate = "${base64decode(local.cluster_ca_certificate)}"
}

resource "kubernetes_deployment" "nginx" {
  metadata {
    name = "nginx"
  }
  spec {
    selector {
      app = "nginx"
    }
    template {
      metadata {
        labels {
          app = "nginx"
        }
      }
      spec {
        container {
          image = "nginx:1.15"
          name  = "app"
          resources {
            requests {
              memory = "100Mi"
              cpu    = "100m"
            }
          }
          port {
            container_port = 80
          }
        }
      }
    }
  }
}

resource "kubernetes_service" "nginx" {
  metadata {
    name = "nginx"
    labels {
      app = "nginx"
    }
  }
  spec {
    type = "NodePort"
    port {
      port        = 80
      target_port = 80
      node_port   = 30080
    }
    selector {
      app = "nginx"
    }
  }
}

resource "google_compute_firewall" "nodeport_http" {
  name    = "nodeport-http"
  network = "${google_container_cluster.main.network}"

  allow {
    protocol = "tcp"
    ports    = ["30080"]
  }
}

data "google_compute_instance_group" "main_cluster" {
  self_link = "${google_container_cluster.main.instance_group_urls.0}"
}

// Not yet available as of v1.18:
// https://github.com/terraform-providers/terraform-provider-google/pull/1906
//
// data "google_compute_instance" "main_cluster_primary" {
//   self_link = "${data.google_compute_instance_group.instances.0}"
// }
