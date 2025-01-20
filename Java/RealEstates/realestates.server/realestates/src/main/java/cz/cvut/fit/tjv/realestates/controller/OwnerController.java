package cz.cvut.fit.tjv.realestates.controller;

import cz.cvut.fit.tjv.realestates.application.OwnerService;
import cz.cvut.fit.tjv.realestates.application.PropertyService;
import cz.cvut.fit.tjv.realestates.controller.converter.DTOConverter;
import cz.cvut.fit.tjv.realestates.controller.dto.OwnerDTO;
import cz.cvut.fit.tjv.realestates.controller.dto.PropertyDTO;
import cz.cvut.fit.tjv.realestates.domain.Owner;
import cz.cvut.fit.tjv.realestates.domain.Property;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("/rest/api/owner")
public class OwnerController {
    private final DTOConverter<OwnerDTO, Owner> ownerDtoConverter;
    private final DTOConverter<PropertyDTO, Property> propertyDtoConverter;
    private final PropertyService propertyService;
    private final OwnerService ownerService;

    public OwnerController(DTOConverter<OwnerDTO, Owner> ownerDtoConverter,
                           PropertyService propertyService,
                           DTOConverter<PropertyDTO, Property> propertyDtoConverter,
                           OwnerService ownerService) {
        this.ownerDtoConverter = ownerDtoConverter;
        this.propertyService = propertyService;
        this.propertyDtoConverter = propertyDtoConverter;
        this.ownerService = ownerService;
    }

    @GetMapping("{id}")
    public OwnerDTO getOwner(@PathVariable("id") String email) {
        return ownerDtoConverter.toDTO(ownerService.getEntityById(email));
    }

    @GetMapping("{id}/properties")
    public List<PropertyDTO> getOwnedProperties(@PathVariable("id") String email) {
        return ownerService.getEntityById(email).getOwnedProperties().stream().map(propertyDtoConverter::toDTO).toList();
    }

    @PutMapping("{id}")
    public OwnerDTO updateOwner(@PathVariable("id") String email, @RequestBody OwnerDTO ownerDto) {
        Owner owner = ownerDtoConverter.toEntity(ownerDto);
        owner.setEmail(email);
        return ownerDtoConverter.toDTO(ownerService.updateEntity(owner));
    }

    @GetMapping
    public List<OwnerDTO> getOwners() {
        return ownerService.getAllEntities().stream().map(ownerDtoConverter::toDTO).toList();
    }

    @PostMapping
    public OwnerDTO createOwner(@RequestBody OwnerDTO ownerDto) {
        return ownerDtoConverter.toDTO(ownerService.createEntity(ownerDtoConverter.toEntity(ownerDto)));
    }

    @DeleteMapping("{id}")
    public void deleteOwner(@PathVariable("id") String email) {
        long propertyCount = ownerService.getEntityById(email).getOwnedProperties().size();
        for (int i = 0; i < propertyCount; i++) {
            propertyService.removeOwnerFromProperty(ownerService.getEntityById(email), ownerService.getEntityById(email).getOwnedProperties().get(0).getId());
        }
        ownerService.deleteEntity(email);
    }

    @PutMapping("{id}/properties")
    public List<PropertyDTO> addOwnerToProperty(@PathVariable("id") String email, @RequestBody PropertyDTO propertyDTO) {
        propertyService.addOwnerToProperty(ownerService.getEntityById(email),propertyDTO.getId());
        return ownerService.getEntityById(email).getOwnedProperties().stream().map(propertyDtoConverter::toDTO).toList();
    }

    @DeleteMapping("{id}/properties/{id_property}")
    public List<PropertyDTO> deleteOwnerFromProperty(@PathVariable("id") String email, @PathVariable("id_property") Long id) {
        propertyService.removeOwnerFromProperty(ownerService.getEntityById(email),id);
        return ownerService.getEntityById(email).getOwnedProperties().stream().map(propertyDtoConverter::toDTO).toList();
    }

}
