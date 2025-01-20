package cz.cvut.fit.tjv.realestates.controller.dto;

import lombok.AllArgsConstructor;
import lombok.Getter;

import java.util.List;

@Getter
@AllArgsConstructor
public class PropertyDTO {
    private final Long id;
    private final Integer propertySize;
    private final Long rentPrice;
    private final String city;
    private final String street;
    private final Integer streetNumber;
    private final String customerEmail;
    private final List<String> ownersEmails;
}
