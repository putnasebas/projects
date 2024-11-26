package cz.cvut.fit.tjv.realestates.application;

import cz.cvut.fit.tjv.realestates.domain.Customer;
import cz.cvut.fit.tjv.realestates.domain.Owner;
import cz.cvut.fit.tjv.realestates.domain.Property;
import cz.cvut.fit.tjv.realestates.repository.JPACustomerRepository;
import cz.cvut.fit.tjv.realestates.repository.JPAOwnerRepository;
import cz.cvut.fit.tjv.realestates.repository.JPAPropertyRepository;
import jakarta.persistence.EntityNotFoundException;
import jakarta.transaction.Transactional;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;

import java.time.LocalDate;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

@SpringBootTest
@Transactional
public class PropertyServiceIntegrationTest {
    @Autowired
    private JPAPropertyRepository propertyRepository;
    @Autowired
    private JPAOwnerRepository ownerRepository;
    @Autowired
    private JPACustomerRepository customerRepository;
    private PropertyService propertyService;

    @BeforeEach
    public void setUp() {
        propertyService = new PropertyServiceImpl(propertyRepository, ownerRepository, customerRepository);
    }

    @Test
    public void testSaveProperty() {
        Property newProperty = new Property(0L, 20, 40_000L,
                "Praha", "Radlicka", 12, null, List.of());
        Owner testOwner = new Owner("RandomOwner@mail.com", null, null);
        newProperty.setOwners(List.of(testOwner));
        ownerRepository.save(testOwner);

        Property savedProperty = propertyService.createEntity(newProperty);
        assertNotNull(savedProperty);
        assertEquals(newProperty.getPropertySize(), savedProperty.getPropertySize());
        assertEquals(newProperty.getRentPrice(), savedProperty.getRentPrice());
        assertEquals(newProperty.getCity(), savedProperty.getCity());
        assertEquals(newProperty.getStreet(), savedProperty.getStreet());
        assertEquals(newProperty.getStreetNumber(), savedProperty.getStreetNumber());
        assertEquals(newProperty.getOwners().stream().map(Owner::getEmail).toList(),
                savedProperty.getOwners().stream().map(Owner::getEmail).toList());
        // Check if property is accessible from database
        Property checkProperty = propertyService.getEntityById(savedProperty.getId());
        assertNotNull(checkProperty);
        assertEquals(newProperty.getPropertySize(), checkProperty.getPropertySize());
        assertEquals(newProperty.getRentPrice(), checkProperty.getRentPrice());
        assertEquals(newProperty.getCity(), checkProperty.getCity());
        assertEquals(newProperty.getStreet(), checkProperty.getStreet());
        assertEquals(newProperty.getStreetNumber(), checkProperty.getStreetNumber());
        assertEquals(newProperty.getOwners().stream().map(Owner::getEmail).toList(),
                checkProperty.getOwners().stream().map(Owner::getEmail).toList());
    }

    @Test
    public void testUpdateProperty() {
        Property oldProperty = new Property(0L, 20, 40_000L,
                "Praha", "Radlicka", 12, null, List.of());
        Owner testOwner = new Owner("RandomOwner@mail.com", null, null);
        oldProperty.setOwners(List.of(testOwner));
        ownerRepository.save(testOwner);

        Property actualSavedOldProperty = propertyService.createEntity(oldProperty);
        assertNotNull(actualSavedOldProperty);
        assertEquals(oldProperty.getRentPrice(), actualSavedOldProperty.getRentPrice());
        Property editedProperty = new Property(actualSavedOldProperty.getId(), 20, 45_000L,
                "Praha", "Radlicka", 12, null, List.of());
        // Check if edit was successful
        propertyService.updateEntity(editedProperty);
        Property actualEditedProperty = propertyService.getEntityById(actualSavedOldProperty.getId());
        assertNotNull(actualEditedProperty);
        assertEquals(editedProperty.getRentPrice(), actualEditedProperty.getRentPrice());
    }

    @Test
    public void testDeleteProperty() {
        Property oldProperty = new Property(0L, 20, 40_000L,
                "Praha", "Radlicka", 12, null, List.of());
        Owner testOwner = new Owner("RandomOwner@mail.com", null, null);
        oldProperty.setOwners(List.of(testOwner));
        ownerRepository.save(testOwner);
        Long propertyId = propertyService.createEntity(oldProperty).getId();

        // Check saved property
        Property savedProperty = propertyService.getEntityById(propertyId);
        assertNotNull(savedProperty);
        assertEquals(oldProperty.getPropertySize(), savedProperty.getPropertySize());
        assertEquals(oldProperty.getRentPrice(), savedProperty.getRentPrice());
        assertEquals(oldProperty.getCity(), savedProperty.getCity());
        assertEquals(oldProperty.getStreet(), savedProperty.getStreet());
        assertEquals(oldProperty.getStreetNumber(), savedProperty.getStreetNumber());
        assertEquals(oldProperty.getOwners().stream().map(Owner::getEmail).toList(),
                savedProperty.getOwners().stream().map(Owner::getEmail).toList());
        // Check Proper deletetion
        propertyService.deleteEntity(propertyId);
        assertThrowsExactly(EntityNotFoundException.class, () -> propertyService.getEntityById(oldProperty.getId()));
    }

    @Test
    public void testPropertyCustomerAddRelation() {
        OwnerService ownerService = new OwnerServiceImpl(ownerRepository);
        CustomerService customerService = new CustomerServiceImpl(customerRepository);
        Owner testOwner = new Owner("RandomOwner@mail.com", null, null);
        Customer testCustomer = new Customer("Java.Java@mail.com", "Java", "Java",
                LocalDate.of(1988, 12, 11), List.of());
        Customer savedCustomer = customerService.createEntity(testCustomer);
        ownerService.createEntity(testOwner);

        // Test add to db
        Property savedProperty = propertyService.createEntity(new Property(0L, 20, 40_000L,
                "Praha", "Radlicka", 12,
                null, List.of(ownerRepository.getReferenceById(testOwner.getEmail()))));
        propertyService.addPropertyToCustomer(savedProperty.getId(), testCustomer.getEmail());
        Property editedProperty = propertyService.getEntityById(savedProperty.getId());
        assertNotNull(editedProperty);
        assertEquals(testCustomer.getEmail(), editedProperty.getCustomer().getEmail());

        Customer actualCustomer = customerService.getEntityById(savedCustomer.getEmail());
        assertNotNull(actualCustomer);
        assertEquals(List.of(editedProperty), actualCustomer.getSubleases());
    }

    @Test
    public void testPropertyOwnerAddRelation() {
        OwnerService ownerService = new OwnerServiceImpl(ownerRepository);
        CustomerService customerService = new CustomerServiceImpl(customerRepository);
        Owner testOwner = new Owner("RandomOwner@mail.com", null, null);
        Customer testCustomer = new Customer("Java.Java@mail.com", "Java", "Java",
                LocalDate.of(1988, 12, 11), List.of());
        Owner testOwnerI = new Owner("TestOwner@mail.com", null, List.of());
        customerService.createEntity(testCustomer);
        ownerService.createEntity(testOwner);
        ownerService.createEntity(testOwnerI);

        // Test add to db
        Property savedProperty = propertyService.createEntity(new Property(0L, 20, 40_000L,
                "Praha", "Radlicka", 12,
                null, List.of(ownerRepository.getReferenceById(testOwner.getEmail()))));
        propertyService.addOwnerToProperty(testOwnerI, savedProperty);
        Property editedProperty = propertyService.getEntityById(savedProperty.getId());
        Owner firstEditedOwner = ownerService.getEntityById(testOwner.getEmail());
        Owner secondEditedOwner = ownerService.getEntityById(testOwnerI.getEmail());

        assertNotNull(editedProperty);
        assertEquals(List.of(testOwner.getEmail(), testOwnerI.getEmail()),
                editedProperty.getOwners().stream().map(Owner::getEmail).toList());
        assertNotNull(firstEditedOwner);
        assertEquals(List.of(savedProperty), firstEditedOwner.getOwnedProperties());
        assertNotNull(secondEditedOwner);
        assertEquals(List.of(savedProperty), secondEditedOwner.getOwnedProperties());
    }

}
