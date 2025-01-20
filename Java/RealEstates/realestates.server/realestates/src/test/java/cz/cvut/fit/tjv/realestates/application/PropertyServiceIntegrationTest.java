package cz.cvut.fit.tjv.realestates.application;

import cz.cvut.fit.tjv.realestates.domain.Customer;
import cz.cvut.fit.tjv.realestates.domain.Owner;
import cz.cvut.fit.tjv.realestates.domain.Property;
import cz.cvut.fit.tjv.realestates.repository.JPACustomerRepository;
import cz.cvut.fit.tjv.realestates.repository.JPAOwnerRepository;
import cz.cvut.fit.tjv.realestates.repository.JPAPropertyRepository;
import jakarta.persistence.EntityNotFoundException;
import jakarta.transaction.Transactional;
import org.junit.jupiter.api.AfterAll;
import org.junit.jupiter.api.AfterEach;
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
    @AfterEach
    public void setUp() {
        propertyRepository.deleteAll();
        ownerRepository.deleteAll();
        customerRepository.deleteAll();
    }

    @Test
    public void testSaveProperty() {
        propertyService = new PropertyServiceImpl(propertyRepository, ownerRepository, customerRepository);
        Property newProperty = new Property(0L, 20, 40_000L,
                "Praha", "Radlicka", 12, null, List.of());
        Owner testOwner = new Owner("RandomOwner@mail.com", null, List.of());
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
        // Check if owner has updated as well
        assertEquals(List.of(newProperty.getCity()),
                 ownerRepository.getReferenceById(testOwner.getEmail()).
                        getOwnedProperties().stream().map(Property::getCity).toList());
    }

    @Test
    public void testUpdateProperty() {
        propertyService = new PropertyServiceImpl(propertyRepository, ownerRepository, customerRepository);
        Property oldProperty = new Property(0L, 20, 40_000L,
                "Praha", "Radlicka", 12, null, List.of());
        Owner testOwner = new Owner("RandomOwner@mail.com", null, List.of());
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
        propertyService = new PropertyServiceImpl(propertyRepository, ownerRepository, customerRepository);
        Property oldProperty = new Property(0L, 20, 40_000L,
                "Praha", "Radlicka", 12, null, List.of());
        Owner testOwner = new Owner("RandomOwner@mail.com", null, List.of());
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
        // Check Proper deletion
        propertyService.deleteEntity(propertyId);
        assertThrowsExactly(EntityNotFoundException.class, () -> propertyService.getEntityById(oldProperty.getId()));
    }

    @Test
    public void testPropertyCustomerAddRelation() {
        propertyService = new PropertyServiceImpl(propertyRepository, ownerRepository, customerRepository);
        OwnerService ownerService = new OwnerServiceImpl(ownerRepository);
        CustomerService customerService = new CustomerServiceImpl(customerRepository, propertyRepository);

        Owner testOwner = new Owner("RandomOwner1@mail.com", null, List.of());
        Customer testCustomer = new Customer("Java.Java1@mail.com", "Java", "Java1",
                LocalDate.of(1988, 12, 11), List.of());
        Customer savedCustomer = customerService.createEntity(testCustomer);
        assertEquals(testCustomer.getEmail(), savedCustomer.getEmail());
        ownerService.createEntity(testOwner);
        assertNotNull(ownerRepository.getReferenceById(testOwner.getEmail()));
        Property savedProperty = propertyService.createEntity(new Property(0L, 20, 40_000L,
                "Praha", "Radlicka", 12,
                null, List.of(testOwner)));

        // Add new relation
        propertyService.addPropertyToCustomer(savedProperty.getId(), testCustomer.getEmail());
        Property editedProperty = propertyService.getEntityById(savedProperty.getId());
        assertNotNull(editedProperty);
        assertEquals(testCustomer.getEmail(), editedProperty.getCustomer().getEmail());
        Customer actualCustomer = customerService.getEntityById(savedCustomer.getEmail());
        assertNotNull(actualCustomer);
        assertEquals(List.of(editedProperty.getId()), actualCustomer.getSubleases().stream().map(Property::getId).toList());
    }

    @Test
    public void testPropertyCustomerRemoveRelation() {
        propertyService = new PropertyServiceImpl(propertyRepository, ownerRepository, customerRepository);
        OwnerService ownerService = new OwnerServiceImpl(ownerRepository);
        CustomerService customerService = new CustomerServiceImpl(customerRepository, propertyRepository);

        Owner testOwner = new Owner("RandomOwner1@mail.com", null, List.of());
        Customer testCustomer = new Customer("Java.Java1@mail.com", "Java", "Java1",
                LocalDate.of(1988, 12, 11), List.of());
        Customer savedCustomer = customerService.createEntity(testCustomer);
        assertEquals(testCustomer.getEmail(), savedCustomer.getEmail());
        ownerService.createEntity(testOwner);
        assertNotNull(ownerRepository.getReferenceById(testOwner.getEmail()));
        Property savedProperty = propertyService.createEntity(new Property(0L, 20, 40_000L,
                "Praha", "Radlicka", 12,
                null, List.of(testOwner)));
        propertyService.removePropertyFromCustomer(savedProperty.getId());
        assertEquals(List.of(), customerService.getEntityById(testCustomer.getEmail()).getSubleases());
        assertNull(propertyService.getEntityById(savedProperty.getId()).getCustomer());
    }

    @Test
    public void testPropertyOwnerAddRelation() {
        propertyService = new PropertyServiceImpl(propertyRepository, ownerRepository, customerRepository);
        OwnerService ownerService = new OwnerServiceImpl(ownerRepository);
        CustomerService customerService = new CustomerServiceImpl(customerRepository, propertyRepository);

        Owner testOwner = new Owner("RandomOwner@mail.com", null, List.of());
        Owner testOwnerI = new Owner("TestOwner@mail.com", null, List.of());
        ownerService.createEntity(testOwner);
        ownerService.createEntity(testOwnerI);

        // Test add to db
        Property savedProperty = propertyService.createEntity(new Property(0L, 20, 40_000L,
                "Praha", "Radlicka", 12,
                null, List.of(ownerRepository.getReferenceById(testOwner.getEmail()))));
        propertyService.addOwnerToProperty(testOwnerI, savedProperty.getId());
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

    @Test
    public void testPropertyOwnerRemoveRelation() {
        PropertyService propertyService = new PropertyServiceImpl(propertyRepository, ownerRepository, customerRepository);
        Owner inputTestFirstOwner = new Owner("RandomOwner@mail.com", null, List.of());
        Owner inputTestSecondOwner = new Owner("TestOwner@mail.com", null, List.of());
        Property inputProperty = new Property(0L, 20, 40_000L,
                "Praha", "Radlicka", 12, null, List.of(inputTestFirstOwner, inputTestSecondOwner));
        ownerRepository.save(inputTestFirstOwner);
        Owner savedSecondOwner = ownerRepository.save(inputTestSecondOwner);
        Property savedProperty = propertyService.createEntity(inputProperty);
        // Check if link between owner and property is removed
        propertyService.removeOwnerFromProperty(savedSecondOwner, savedProperty.getId());
        Property actualProperty = propertyService.getEntityById(savedProperty.getId());
        assertNotNull(actualProperty);
        assertEquals(List.of(inputTestFirstOwner.getEmail()), actualProperty.getOwners().stream().map(Owner::getEmail).toList());
        assertEquals(List.of(), savedSecondOwner.getOwnedProperties());
    }

    @Test
    public void testRemovePropertyWhenLastOwnerIsRemoved() {
        PropertyService propertyService = new PropertyServiceImpl(propertyRepository, ownerRepository, customerRepository);
        Owner testInputOwner = new Owner("RandomOwner@mail.com", null, List.of());
        Property inputProperty = new Property(0L, 20, 40_000L,
                "Praha", "Radlicka", 12, null, List.of(testInputOwner));
        Customer testInputCustomer = new Customer("Java.Java@mail.com", "Java", "Java",
                LocalDate.of(1988, 12, 11), List.of());
        customerRepository.save(testInputCustomer);
        Owner savedOwner = ownerRepository.save(testInputOwner);
        Property savedProperty = propertyService.createEntity(inputProperty);
        propertyService.addPropertyToCustomer(savedProperty.getId(), testInputCustomer.getEmail());
        // Check if link between customer and removed property is removed
        propertyService.removeOwnerFromProperty(savedOwner, savedProperty.getId());
        assertThrowsExactly(EntityNotFoundException.class, () -> propertyService.getEntityById(savedProperty.getId()));
        assertEquals(List.of(), savedOwner.getOwnedProperties());
        assertEquals(List.of(), customerRepository.getReferenceById(testInputCustomer.getEmail()).getSubleases());
    }

}
