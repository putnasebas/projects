package cz.cvut.fit.tjv.realestates.controller;

import cz.cvut.fit.tjv.realestates.application.OwnerService;
import cz.cvut.fit.tjv.realestates.application.PropertyService;
import cz.cvut.fit.tjv.realestates.controller.converter.CustomerDTOConverter;
import cz.cvut.fit.tjv.realestates.controller.converter.OwnerDTOConverter;
import cz.cvut.fit.tjv.realestates.controller.converter.PropertyDTOConverter;
import cz.cvut.fit.tjv.realestates.controller.dto.CustomerDTO;
import cz.cvut.fit.tjv.realestates.controller.dto.OwnerDTO;
import cz.cvut.fit.tjv.realestates.controller.dto.PropertyDTO;
import cz.cvut.fit.tjv.realestates.domain.Customer;
import cz.cvut.fit.tjv.realestates.domain.Owner;
import cz.cvut.fit.tjv.realestates.domain.Property;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.responses.ApiResponse;
import io.swagger.v3.oas.annotations.responses.ApiResponses;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("/rest/api/property")
public class PropertyController {
    private final PropertyService propertyService;
    private final PropertyDTOConverter propertyDTOConverter;
    private final OwnerDTOConverter ownerDTOConverter;
    private final CustomerDTOConverter customerDTOConverter;
    private final OwnerService ownerService;

    public PropertyController(PropertyService propertyService,
                              PropertyDTOConverter propertyDTOConverter,
                              OwnerDTOConverter ownerDTOConverter,
                              CustomerDTOConverter customerDTOConverter,
                              OwnerService ownerService) {
        this.propertyService = propertyService;
        this.propertyDTOConverter = propertyDTOConverter;
        this.ownerDTOConverter = ownerDTOConverter;
        this.customerDTOConverter = customerDTOConverter;
        this.ownerService = ownerService;
    }

    @GetMapping("{id}")
    public PropertyDTO getProperty(@PathVariable Long id) {
        return propertyDTOConverter.toDTO(propertyService.getEntityById(id));
    }

    @GetMapping("{id}/owners")
    public List<OwnerDTO> getPropertyOwners(@PathVariable Long id) {
        return propertyService.getEntityById(id).getOwners().stream().map(ownerDTOConverter::toDTO).toList();
    }

    @GetMapping
    public List<PropertyDTO> getProperties() {
        return propertyService.getAllEntities().stream().map(propertyDTOConverter::toDTO).toList();
    }

    @PostMapping
    public PropertyDTO createProperty(@RequestBody PropertyDTO propertyDTO) {
        return propertyDTOConverter.toDTO(propertyService.createEntity(propertyDTOConverter.toEntity(propertyDTO)));
    }

    @PutMapping("{id}")
    public PropertyDTO updateProperty(@PathVariable Long id, @RequestBody PropertyDTO propertyDTO) {
        Property updatedProperty = propertyDTOConverter.toEntity(propertyDTO);
        updatedProperty.setId(id);
        return propertyDTOConverter.toDTO(propertyService.updateEntity(updatedProperty));
    }

    @DeleteMapping("{id}")
    public void deleteProperty(@PathVariable Long id) {
        propertyService.deleteEntity(id);
    }

    @Operation(summary = "Gets all properties that are being owned by both owners")
    @ApiResponses(value = {
        @ApiResponse(responseCode = "200", description = "Both owners do have an intersection of owned properties"),
        @ApiResponse(responseCode = "400", description = "At least one of given Ids is invalid")
    })
    @GetMapping("owners/intersection")
    public List<PropertyDTO> getPropertyIntersection(@RequestParam("o1")String emailFirstOwner, @RequestParam("o2") String emailSecondOwner) {
        Owner firstOwner = ownerService.getEntityById(emailFirstOwner);
        Owner secondOwner = ownerService.getEntityById(emailSecondOwner);
        return propertyService.getPropertiesOwnedByBothOwners(firstOwner, secondOwner).stream().map(propertyDTOConverter::toDTO).toList();
    }

    @PutMapping("{id}/customer")
    public CustomerDTO addCustomerToProperty(@PathVariable Long id, @RequestBody CustomerDTO customerDTO) {
        Customer customer = propertyService.addPropertyToCustomer(id, customerDTO.getEmail());
        return customerDTOConverter.toDTO(customer);
    }

    @DeleteMapping("{id}/customer")
    public CustomerDTO deleteCustomerFromProperty(@PathVariable Long id) {
        Customer customer = propertyService.removePropertyFromCustomer(id);
        return customerDTOConverter.toDTO(customer);
    }

    @PutMapping("{id}/owners")
    public List<OwnerDTO> addPropertyToOwners(@PathVariable("id") Long id, @RequestBody OwnerDTO ownerDTO) {
        Owner owner = ownerDTOConverter.toEntity(ownerDTO);
        propertyService.addOwnerToProperty(owner,id);
        return propertyService.getEntityById(id).getOwners().stream().map(ownerDTOConverter::toDTO).toList();
    }

    @DeleteMapping("{id}/owners/{id_owner}")
    public void deletePropertyFromOwner(@PathVariable("id") Long id, @PathVariable("id_owner") String email) {
        propertyService.removeOwnerFromProperty(ownerService.getEntityById(email), id);
    }

}
