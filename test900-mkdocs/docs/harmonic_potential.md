# `harmonic_potential`

Three-diomensional harmonic oscillator potential.

```c++
struct harmonic_potential
{
    md::scalar spring_constant = 1;

    md::scalar evaluate_energy(md::vector r) const;
    md::vector evaluate_force(md::vector r) const;
};
```


## Member functions

| member functions                      ||
|---------------------------------------||
| [`evaluate_energy`](#evaluate_energy) | evaluates potential energy |
| [`evaluate_force`](#evaluate_force)   | evaluates potential force |

### `evaluate_energy`

```c++
md::scalar evaluate_energy (
    md::vector r
) const
```

Computes potential energy at given displacement `r`.

#### Parameters

- `r` - is the displacement vector at which the energy is evaluated.

#### Returns

Potential energy value.


### `evaluate_force`

```c++
md::vector evaluate_force (
    md::vector r
) const
```

Computes potential force at given displacement `r`.

#### Parameters

- `r` - is the displacement vector at which the force is evaluated.

#### Returns

Potential force as a three-dimensional vector.
