package cz.cvut.fit.tjv.realestates.domain;

import jakarta.persistence.*;
import jakarta.validation.constraints.NotBlank;
import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import java.time.LocalDate;
import java.util.List;

@Entity
@Getter
@Setter
@NoArgsConstructor
@AllArgsConstructor
public class Customer {
    @Id
    private String email;
    @Column(name = "first_name")
    @NotBlank(message = "first_name cannot be blank")
    private String firstName;
    @Column(name = "last_name")
    @NotBlank(message = "last_name cannot be blank")
    private String lastName;
    @Column(name = "birth_date")
    private LocalDate birthDate;

    @OneToMany(
            fetch = FetchType.LAZY,
            targetEntity = Property.class,
            mappedBy = "customer",
            cascade = {CascadeType.PERSIST, CascadeType.MERGE}
    )
    private List<Property> subleases = List.of();
}
