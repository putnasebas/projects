package cz.cvut.fit.tjv.realestates.domain;

import jakarta.persistence.*;
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
public class Owner {
    @Id
    private String email;
    @Column(nullable = true)
    private String phoneNumber;

    @ManyToMany(
            fetch = FetchType.LAZY,
            cascade = CascadeType.REFRESH,
            mappedBy = "owners"
    )
    private List<Property> ownedProperties;
}
