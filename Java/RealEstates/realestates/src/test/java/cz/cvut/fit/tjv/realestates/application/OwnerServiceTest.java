package cz.cvut.fit.tjv.realestates.application;

import cz.cvut.fit.tjv.realestates.domain.Owner;
import cz.cvut.fit.tjv.realestates.repository.JPAOwnerRepository;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.mockito.Mock;

import java.util.List;
import java.util.Optional;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

public class OwnerServiceTest {
    @Mock
    JPAOwnerRepository ownerRepository;

    @BeforeEach
    public void setUp() {
        ownerRepository = mock(JPAOwnerRepository.class);
    }

    private static List<Owner> prepareOwners(List<String> emails) {
        return List.of(
                new Owner(emails.get(0), null, List.of()),
                new Owner(emails.get(1), "752656210", List.of()),
                new Owner(emails.get(2), null, List.of())
        );
    }

    @Test
    public void testGetOwnerById() {
        Owner owner = new Owner("Owner.Owner@mail.com", null, List.of());
        when(ownerRepository.findById(owner.getEmail())).thenReturn(Optional.of(owner));

        OwnerService ownerService = new OwnerServiceImpl(ownerRepository);
        Owner savedOwner = ownerService.getEntityById(owner.getEmail());
        assertNotNull(savedOwner);
        assertEquals(owner, savedOwner);
    }

    @Test
    public void testGetMultipleCustomersById() {
        List<String> emails = List.of("First.Owner@mail.com", "Second.Owner@mail.com", "Third.Owner@mail.com");
        List<Owner> expectedOwners = prepareOwners(emails);
        for (Owner owner : expectedOwners) {
            when(ownerRepository.existsById(owner.getEmail())).thenReturn(true);
            when(ownerRepository.findById(owner.getEmail())).thenReturn(Optional.of(owner));
        }
        OwnerService ownerService = new OwnerServiceImpl(ownerRepository);
        List<Owner> actualOwners = ownerService.getEntitiesByIds(emails);
        assertNotNull(actualOwners);
        assertEquals(expectedOwners, actualOwners);
    }

    @Test
    public void testGetAllOwners() {
        List<String> emails = List.of("First.Owner@mail.com", "Second.Owner@mail.com", "Third.Owner@mail.com");
        List<Owner> expectedOwners = prepareOwners(emails);
        when(ownerRepository.findAll()).thenReturn(expectedOwners);
        OwnerService ownerService = new OwnerServiceImpl(ownerRepository);
        List<Owner> actualOwners = ownerService.getAllEntities();
        assertNotNull(actualOwners);
        assertEquals(expectedOwners, actualOwners);
    }

    @Test
    public void testCreateOwner() {
        Owner newOwner = new Owner("Owner@mail.com", null, List.of());
        when(ownerRepository.save(newOwner)).thenReturn(newOwner);
        OwnerService ownerService = new OwnerServiceImpl(ownerRepository);
        Owner savedOwner = ownerService.createEntity(newOwner);
        assertNotNull(savedOwner);
        assertEquals(newOwner, savedOwner);
    }

    @Test
    public void testCreateInvalidOwner() throws RuntimeException {
        Owner invalidOwner = new Owner("Owmer@randomail.com", null, List.of());
        OwnerService ownerService = new OwnerServiceImpl(ownerRepository);
        assertThrows(RuntimeException.class, () -> ownerService.createEntity(invalidOwner));
    }

    @Test
    public void testUpdateOwner() {
        Owner expectedNewOwner = new Owner("NewOner@mail.com", null, List.of());
        when(ownerRepository.existsById(expectedNewOwner.getEmail())).thenReturn(true);
        when(ownerRepository.save(expectedNewOwner)).thenReturn(expectedNewOwner);
        OwnerService ownerService = new OwnerServiceImpl(ownerRepository);
        Owner savedOwner = ownerService.updateEntity(expectedNewOwner);
        assertNotNull(savedOwner);
        assertEquals(expectedNewOwner, savedOwner);
    }

}
