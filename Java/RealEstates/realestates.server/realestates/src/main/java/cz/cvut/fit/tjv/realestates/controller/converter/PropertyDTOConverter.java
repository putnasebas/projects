package cz.cvut.fit.tjv.realestates.controller.converter;

import cz.cvut.fit.tjv.realestates.application.CustomerService;
import cz.cvut.fit.tjv.realestates.application.OwnerService;
import cz.cvut.fit.tjv.realestates.controller.dto.PropertyDTO;
import cz.cvut.fit.tjv.realestates.domain.Owner;
import cz.cvut.fit.tjv.realestates.domain.Property;
import org.springframework.stereotype.Component;

import java.util.List;

@Component
public class PropertyDTOConverter implements DTOConverter<PropertyDTO, Property> {
    private final OwnerService ownerService;
    private final CustomerService customerService;

    public PropertyDTOConverter(OwnerService ownerService, CustomerService customerService) {
        this.ownerService = ownerService;
        this.customerService = customerService;
    }

    @Override
    public PropertyDTO toDTO(Property property) {
        if(property.getCustomer() != null) {
            return new PropertyDTO(property.getId(), property.getPropertySize(), property.getRentPrice(),
                    property.getCity(), property.getStreet(), property.getStreetNumber(),
                    property.getCustomer().getEmail(), property.getOwners().stream().map(Owner::getEmail).toList());
        }
        return new PropertyDTO(property.getId(), property.getPropertySize(), property.getRentPrice(),
                property.getCity(), property.getStreet(), property.getStreetNumber(),
                null, property.getOwners().stream().map(Owner::getEmail).toList());
    }

    @Override
    public Property toEntity(PropertyDTO propertyDTO) {
        if(propertyDTO.getCustomerEmail() != null) {
            return new Property(propertyDTO.getId(), propertyDTO.getPropertySize(), propertyDTO.getRentPrice(),
                    propertyDTO.getCity(), propertyDTO.getStreet(), propertyDTO.getStreetNumber(),
                    customerService.getEntityById(propertyDTO.getCustomerEmail()),
                    ownerService.getEntitiesByIds(propertyDTO.getOwnersEmails()));
        }
        return new Property(propertyDTO.getId(), propertyDTO.getPropertySize(), propertyDTO.getRentPrice(),
                propertyDTO.getCity(), propertyDTO.getStreet(), propertyDTO.getStreetNumber(),
                null, ownerService.getEntitiesByIds(propertyDTO.getOwnersEmails()));
    }
}
