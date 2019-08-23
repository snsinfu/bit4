Failed. Could not make it work.

Well, I should have not tried to manage Postgres on kubernetes cluster. It is
not the right tool for the first place. DB should be simply installed on a
machine in traditional way.

- It is just overly complicated make database service work on kubernetes.
- Kubernetes is all about quickly deploying and updating apps that scale
  horizontally. DB service, however, is rarely updated and scales vertically.
- Some claim kubernetes persistent volume is not stable.
