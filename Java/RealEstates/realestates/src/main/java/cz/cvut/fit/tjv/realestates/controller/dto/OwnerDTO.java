package cz.cvut.fit.tjv.realestates.controller.dto;

import lombok.AllArgsConstructor;
import lombok.Getter;

import java.util.List;

@Getter
@AllArgsConstructor
public class OwnerDTO {
    private final String email;
    private final String phoneNumber;
    private final List<Long> ownedPropertiesId;
}
