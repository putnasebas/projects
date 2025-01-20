package cz.cvut.fit.tjv.realestates.client.model;

import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import java.util.List;

@Getter
@Setter
@AllArgsConstructor
@NoArgsConstructor
public class OwnerDTO {
    private String email;
    private String phoneNumber;
    private List<Long> ownedPropertiesId = List.of();
}
