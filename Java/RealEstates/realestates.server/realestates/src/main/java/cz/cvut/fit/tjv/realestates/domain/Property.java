package cz.cvut.fit.tjv.realestates.domain;

import jakarta.persistence.*;
import jakarta.validation.constraints.NotBlank;
import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import java.util.List;

@Entity
@Getter
@Setter
@NoArgsConstructor
@AllArgsConstructor
public class Property {
    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE)
    @Column(name ="id_property")
    private Long id;
    @Column(name = "property_size")
    private Integer propertySize;
    @Column(name = "rent_price")
    private Long rentPrice;
    @NotBlank(message = "Property must be located somewhere")
    private String city;
    private String street;
    private Integer streetNumber;

    @ManyToOne(fetch = FetchType.LAZY, targetEntity = Customer.class)
    @JoinColumn(name = "email")
    private Customer customer;

    @ManyToMany(fetch = FetchType.LAZY, targetEntity = Owner.class)
    @JoinTable(
            name = "owned_by",
            joinColumns = @JoinColumn(name = "email"),
            inverseJoinColumns = @JoinColumn(name = "id_property")
    )
    private List<Owner> owners;
}
