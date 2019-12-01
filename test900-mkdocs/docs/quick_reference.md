# Quick Reference

| Potential Classes                                                                     ||
|----------------------|-----------------------------------------------------------------|
| [`harmonic_potential`](#harmonic_potential) | attractive harmonic oscillator potential |
| [`softcore_potrntial`](#softcore_potential) | repulsive bell-shaped potential          |
| [`softwell_potential`](#softwell_potential) | attractive step-shaped potential         |


## Potential Classes

### `harmonic_potential`

```c++
struct harmonic_potential {
    md::scalar spring_constant = 1;
};
```

[Reference](harmonic_potential.md)


### `softcore_potential`

```c++
struct softcore_potential {
    md::scalar energy   = 1;
    md::scalar diameter = 1;
};
```


### `softwell_potential`

```c++
struct softwell_potential {
    md::scalar energy         = 1;
    md::scalar decay_distance = 1;
};
```
