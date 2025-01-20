package cz.cvut.fit.tjv.realestates.application;


import cz.cvut.fit.tjv.realestates.domain.Owner;
import cz.cvut.fit.tjv.realestates.domain.Property;
import cz.cvut.fit.tjv.realestates.repository.JPACustomerRepository;
import cz.cvut.fit.tjv.realestates.repository.JPAOwnerRepository;
import cz.cvut.fit.tjv.realestates.repository.JPAPropertyRepository;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.mockito.Mock;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.any;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

public class PropertyServiceTest {
    @Mock
    private JPAPropertyRepository propertyRepository;
    @Mock
    private JPACustomerRepository customerRepository;
    @Mock
    private JPAOwnerRepository ownerRepository;
    private PropertyService propertyService;

    @BeforeEach
    public void setUp() {
        propertyRepository = mock(JPAPropertyRepository.class);
        customerRepository = mock(JPACustomerRepository.class);
        ownerRepository = mock(JPAOwnerRepository.class);
    }

    private static List<Property> setupProperties() {
        return List.of(
                new Property(0L, 10, 24_000L,
                        "Praha", "Nachodska", 12, null, List.of()),
                new Property(1L, 15, 30_000L,
                        "Praha", "Liberecka", 20, null, List.of())
        );
    }

    @Test
    public void testGetPropertyById() {
        Property expectedProperty = new Property(0L, 10, 24_000L,
                "Praha", "Nachodska", 12, null, List.of());
        when(propertyRepository.findById(expectedProperty.getId())).thenReturn(Optional.of(expectedProperty));
        propertyService = new PropertyServiceImpl(propertyRepository, ownerRepository, customerRepository);

        Property actualProperty = propertyService.getEntityById(expectedProperty.getId());
        assertNotNull(actualProperty);
        assertEquals(expectedProperty, actualProperty);
    }

    @Test
    public void testGetMultiplePropertiesById() {
        List<Property> expectedProperties = setupProperties();
        propertyService = new PropertyServiceImpl(propertyRepository, ownerRepository, customerRepository);
        for(Property property : expectedProperties) {
            when(propertyRepository.findById(property.getId())).thenReturn(Optional.of(property));
            when(propertyRepository.existsById(property.getId())).thenReturn(true);
        }

        List<Property> actualProperties = propertyService.getEntitiesByIds(expectedProperties.stream().map(Property::getId).toList());
        assertNotNull(actualProperties);
        assertEquals(expectedProperties.size(), actualProperties.size());
        assertEquals(expectedProperties, actualProperties);
    }

    @Test
    public void testGetAllProperties() {
        List<Property> expectedProperties = setupProperties();
        propertyService = new PropertyServiceImpl(propertyRepository, ownerRepository, customerRepository);
        when(propertyRepository.findAll()).thenReturn(expectedProperties);
        List<Property> actualProperties = propertyService.getAllEntities();
        assertNotNull(actualProperties);
        assertEquals(expectedProperties.size(), actualProperties.size());
        assertEquals(expectedProperties, actualProperties);
    }

    @Test
    public void testCreateProperty() {
        Property newProperty = setupProperties().get(0);
        Owner propertyOwner = new Owner("Property.Owner@mail.com", null, new ArrayList<>());
        newProperty.setOwners(List.of(propertyOwner));
        propertyService = new PropertyServiceImpl(propertyRepository, ownerRepository, customerRepository);
        when(propertyRepository.save(newProperty)).thenReturn(newProperty);

        when(ownerRepository.existsById(propertyOwner.getEmail())).thenReturn(true);
        when(ownerRepository.getReferenceById(propertyOwner.getEmail())).thenReturn(propertyOwner);
        when(ownerRepository.save(propertyOwner)).thenReturn(propertyOwner);
        Property actualSavedProperty = propertyService.createEntity(newProperty);
        assertNotNull(actualSavedProperty);
        assertEquals(newProperty, actualSavedProperty);
    }

    @Test
    public void testCreateInvalidProperty() {
        Property invalidNewProperty = setupProperties().get(0);
        propertyService = new PropertyServiceImpl(propertyRepository, ownerRepository, customerRepository);
        when(propertyRepository.save(invalidNewProperty)).thenReturn(invalidNewProperty);
        when(propertyRepository.save(invalidNewProperty)).thenReturn(invalidNewProperty);

        assertThrowsExactly(IllegalArgumentException.class, () -> propertyService.createEntity(invalidNewProperty));
    }

    @Test
    public void testUpdateProperty() {
        Property newProperty = setupProperties().get(0);
        propertyService = new PropertyServiceImpl(propertyRepository, ownerRepository, customerRepository);
        when(propertyRepository.existsById(newProperty.getId())).thenReturn(true);
        when(propertyRepository.getReferenceById(newProperty.getId())).thenReturn(newProperty);
        when(propertyRepository.save(any(Property.class))).thenReturn(newProperty);

        Property actualSavedProperty = propertyService.updateEntity(newProperty);
        assertNotNull(actualSavedProperty);
        assertEquals(newProperty, actualSavedProperty);
    }
}
