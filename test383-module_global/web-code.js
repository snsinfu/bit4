var p = document.createElement('p');
p.appendChild(document.createTextNode('hello'));
document.body.appendChild(p);

var q = document.createElement('p');
q.appendChild(document.createTextNode(JSON.stringify(getComputedStyle(p))));
document.body.appendChild(q);
