package cz.cvut.fit.tjv.realestates.controller.converter;

import cz.cvut.fit.tjv.realestates.application.PropertyService;
import cz.cvut.fit.tjv.realestates.controller.dto.OwnerDTO;
import cz.cvut.fit.tjv.realestates.domain.Owner;
import cz.cvut.fit.tjv.realestates.domain.Property;
import org.springframework.stereotype.Component;

@Component
public class OwnerDTOConverter implements  DTOConverter<OwnerDTO, Owner> {
    private final PropertyService propertyService;

    public OwnerDTOConverter(PropertyService propertyService) {
        this.propertyService = propertyService;
    }

    @Override
    public OwnerDTO toDTO(Owner owner) {
        return new OwnerDTO(owner.getEmail(), owner.getPhoneNumber(),
                owner.getOwnedProperties().stream().map(Property::getId).toList());
    }

    @Override
    public Owner toEntity(OwnerDTO ownerDTO) {
        return new Owner(ownerDTO.getEmail(),ownerDTO.getPhoneNumber(),
                propertyService.getEntitiesByIds(ownerDTO.getOwnedPropertiesId()));
    }
}
