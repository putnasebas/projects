package cz.cvut.fit.tjv.realestates.controller.converter;

import cz.cvut.fit.tjv.realestates.application.CustomerService;
import cz.cvut.fit.tjv.realestates.application.OwnerService;
import cz.cvut.fit.tjv.realestates.controller.dto.PropertyDTO;
import cz.cvut.fit.tjv.realestates.domain.Customer;
import cz.cvut.fit.tjv.realestates.domain.Owner;
import cz.cvut.fit.tjv.realestates.domain.Property;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.mockito.Mock;

import java.util.List;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

public class PropertyMapperTest {
    @Mock
    OwnerService ownerService;
    @Mock
    CustomerService customerService;
    PropertyDTOConverter propertyDTOConverter;

    @BeforeEach
    public void setUp() {
        ownerService = mock(OwnerService.class);
        customerService = mock(CustomerService.class);
    }

    @Test
    public void testPropertyToPropertyDTO() {
        Owner testOwner = new Owner();
        testOwner.setEmail("testOwmer@test.com");
        Customer testCustomer = new Customer();
        testCustomer.setEmail("testCustomer@test.com");
        Property property = new Property(1L, 200, 30_000L,
                "Praha", "Delnicka", 12, testCustomer, List.of(testOwner));
        PropertyDTO expectedPropertyDTO = new PropertyDTO(1L, 200, 30_000L,
                "Praha", "Delnicka", 12, testCustomer.getEmail(), List.of(testOwner.getEmail()));
        propertyDTOConverter = new PropertyDTOConverter(ownerService, customerService);

        PropertyDTO actualPropertyDTO = propertyDTOConverter.toDTO(property);
        assertNotNull(actualPropertyDTO);
        assertEquals(expectedPropertyDTO.getId(), actualPropertyDTO.getId());
        assertEquals(expectedPropertyDTO.getPropertySize(), actualPropertyDTO.getPropertySize());
        assertEquals(expectedPropertyDTO.getRentPrice(), actualPropertyDTO.getRentPrice());
        assertEquals(expectedPropertyDTO.getCity(), actualPropertyDTO.getCity());
        assertEquals(expectedPropertyDTO.getStreet(), actualPropertyDTO.getStreet());
        assertEquals(expectedPropertyDTO.getStreetNumber(), actualPropertyDTO.getStreetNumber());
        assertEquals(expectedPropertyDTO.getCustomerEmail(), actualPropertyDTO.getCustomerEmail());
        assertEquals(expectedPropertyDTO.getOwnersEmails(), actualPropertyDTO.getOwnersEmails());
    }

    @Test
    public void testPropertyDTOToProperty() {
        Owner testOwner = new Owner();
        testOwner.setEmail("testOwmer@test.com");
        Customer testCustomer = new Customer();
        testCustomer.setEmail("testCustomer@test.com");
        PropertyDTO propertyDTO = new PropertyDTO(1L, 200, 30_000L,
                "Praha", "Delnicka", 12, testCustomer.getEmail(), List.of(testOwner.getEmail()));
        Property expectedProperty = new Property(1L, 200, 30_000L,
                "Praha", "Delnicka", 12, testCustomer, List.of(testOwner));
        propertyDTOConverter = new PropertyDTOConverter(ownerService, customerService);
        when(ownerService.getEntitiesByIds(propertyDTO.getOwnersEmails())).thenReturn(expectedProperty.getOwners());
        when(customerService.getEntityById(propertyDTO.getCustomerEmail())).thenReturn(testCustomer);

        Property actualProperty = propertyDTOConverter.toEntity(propertyDTO);
        assertNotNull(actualProperty);
        assertEquals(expectedProperty.getId(), actualProperty.getId());
        assertEquals(expectedProperty.getPropertySize(), actualProperty.getPropertySize());
        assertEquals(expectedProperty.getRentPrice(), actualProperty.getRentPrice());
        assertEquals(expectedProperty.getCity(), actualProperty.getCity());
        assertEquals(expectedProperty.getStreet(), actualProperty.getStreet());
        assertEquals(expectedProperty.getStreetNumber(), actualProperty.getStreetNumber());
        assertEquals(expectedProperty.getCustomer(), actualProperty.getCustomer());
        assertEquals(expectedProperty.getOwners(), actualProperty.getOwners());
    }

}
