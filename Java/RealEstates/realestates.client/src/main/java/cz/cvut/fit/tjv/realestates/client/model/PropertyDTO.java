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
public class PropertyDTO {
    private Long id;
    private Integer propertySize;
    private Long rentPrice;
    private String city;
    private String street;
    private Integer streetNumber;
    private String customerEmail;
    private List<String> ownersEmails;

}
